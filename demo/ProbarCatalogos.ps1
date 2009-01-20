$ErrorActionPreference = 'Stop'
$name = 'prueba-sdgcontable-mysql'
$image = 'mysql@sha256:4bc6bc963e6d8443453676cae56536f4b8156d78bae03c0145cbe47c2aad73bb'
$database = 'sdgcontable_demo'
$sql = Join-Path $PSScriptRoot 'catalogos.sql'
$client = 'MYSQL_PWD="$MYSQL_ROOT_PASSWORD" exec mysql --user=root --database "$MYSQL_DATABASE" --batch --skip-column-names'
$previousPassword = $env:MYSQL_ROOT_PASSWORD
$previousDemoPassword = $env:MYSQL_PASSWORD
$owned = $null

try {
    $env:MYSQL_ROOT_PASSWORD = [guid]::NewGuid().ToString('N')
    $env:MYSQL_PASSWORD = [guid]::NewGuid().ToString('N')
    $created = docker run --rm -d --name $name -p '127.0.0.1::3306' -e "MYSQL_DATABASE=$database" -e MYSQL_ROOT_PASSWORD -e 'MYSQL_USER=consulta_demo' -e MYSQL_PASSWORD $image
    if ($LASTEXITCODE -or $created -notmatch '^[0-9a-f]{64}$') {
        throw "Could not create the isolated MySQL instance. / No se pudo crear la instancia de prueba."
    }
    $owned = $created
    $ready = $false
    foreach ($attempt in 1..45) {
        'SELECT 1;' | docker exec -i $owned sh -c "$client --protocol=TCP --host=127.0.0.1" 2>$null | Out-Null
        if ($LASTEXITCODE -eq 0) {
            $ready = $true
            break
        }
        Start-Sleep -Milliseconds 650
    }
    if (-not $ready) { throw 'The isolated MySQL instance did not become ready.' }
    Get-Content -LiteralPath $sql -Raw | docker exec -i $owned sh -c $client
    if ($LASTEXITCODE) { throw 'The fictional schema could not be loaded.' }

    $query = @'
SELECT 'cliente', id, nombre FROM cliente WHERE id=101
UNION ALL SELECT 'proveedor', id, nombre FROM proveedor WHERE id=201
UNION ALL SELECT 'grupo', id, nombre FROM grupo WHERE id=301
UNION ALL SELECT 'permisos', grupo, clase FROM permisos WHERE grupo=301 AND permiso=1 AND clase='CClientesView'
UNION ALL SELECT 'usuario', id, nombre FROM usuario WHERE id=401
UNION ALL SELECT 'shared', id, nombre FROM shared WHERE id=501;
'@
    $records = $query | docker exec -i $owned sh -c $client
    $expect = @(
        "cliente`t101`tComercio ficticio", "proveedor`t201`tMateriales de ejemplo",
        "grupo`t301`tConsulta demo", "permisos`t301`tCClientesView",
        "usuario`t401`tUsuario ficticio", "shared`t501`tCategoria demo"
    )
    if ($LASTEXITCODE -or (Compare-Object @($records) $expect)) {
        throw "The fictional records do not match. / Los registros ficticios no coinciden."
    }
    $demoClient = 'MYSQL_PWD="$MYSQL_PASSWORD" exec mysql --protocol=TCP --host=127.0.0.1 --user="$MYSQL_USER" --database "$MYSQL_DATABASE" --batch --skip-column-names'
    $loginQuery = "SELECT login, grupo, (SELECT COUNT(*) FROM usuariosh WHERE usuario=usuarios.id) FROM usuarios WHERE login='consulta_demo' AND (estado!=-1 OR estado IS NULL);"
    $login = $loginQuery | docker exec -i $owned sh -c $demoClient
    if ($LASTEXITCODE -or $login -cne "consulta_demo`t301`t0") {
        throw 'The fictional login query failed. / Fallo la consulta de entrada ficticia.'
    }
    $binding = docker port $owned '3306/tcp'
    if ($LASTEXITCODE -or $binding -notmatch '^127\.0\.0\.1:\d+$') {
        throw 'Unexpected MySQL port binding.'
    }
    "$name $binding"
    $records
    "usuarios/usuariosh`t$login"
}
finally {
    $env:MYSQL_ROOT_PASSWORD = $previousPassword
    $env:MYSQL_PASSWORD = $previousDemoPassword
    if ($owned) {
        docker stop --time 2 $owned | Out-Null
        if ($LASTEXITCODE) { throw 'The test instance could not be stopped.' }
    }
}
