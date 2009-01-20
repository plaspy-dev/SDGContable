-- Fictional catalog sample inferred from Administrador / Ejemplo ficticio derivado de Administrador.
CREATE TABLE cliente (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nombre VARCHAR(120) NOT NULL,
    descripcion VARCHAR(160),
    identificacion VARCHAR(30),
    telefono VARCHAR(30),
    direccion VARCHAR(140),
    email VARCHAR(120),
    contacto VARCHAR(120),
    estado INT DEFAULT 1
);
CREATE TABLE proveedor LIKE cliente;
CREATE TABLE grupo (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nombre VARCHAR(120) NOT NULL,
    descripcion VARCHAR(160)
);
CREATE TABLE permisos (
    grupo INT NOT NULL,
    clase VARCHAR(80) NOT NULL,
    permiso INT NOT NULL,
    PRIMARY KEY (grupo, clase)
);
CREATE TABLE usuario (
    id INT PRIMARY KEY AUTO_INCREMENT,
    login VARCHAR(80) NOT NULL,
    nombre VARCHAR(120) NOT NULL,
    grupo INT NOT NULL,
    estado INT DEFAULT 1
);
CREATE TABLE usuarios (
    id INT PRIMARY KEY AUTO_INCREMENT,
    login VARCHAR(80) NOT NULL UNIQUE,
    nombre VARCHAR(120) NOT NULL,
    descripcion VARCHAR(160),
    grupo INT NOT NULL,
    vigencia DATETIME NULL,
    expira INT DEFAULT 0,
    ucambio DATETIME NULL,
    acceso DATETIME NULL,
    autonumerico INT DEFAULT 0,
    estado INT DEFAULT 1
);
CREATE TABLE usuariosh (
    usuario INT NOT NULL,
    dia INT NOT NULL,
    entrada INT NOT NULL,
    salida INT NOT NULL,
    PRIMARY KEY (usuario, dia, entrada)
);
CREATE TABLE shared (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nReservado INT,
    szReservado VARCHAR(40),
    nombre VARCHAR(120) NOT NULL,
    descripcion VARCHAR(160)
);
INSERT INTO cliente (id,nombre,descripcion,identificacion,telefono,direccion,email,contacto)
VALUES (101,'Comercio ficticio','Registro de ejemplo','DEMO-101','000-0101','Calle Demo 101','cliente@example.invalid','Contacto ficticio');
INSERT INTO proveedor (id,nombre,descripcion)
VALUES (201,'Materiales de ejemplo','Proveedor ficticio');
INSERT INTO grupo (id,nombre,descripcion)
VALUES (301,'Consulta demo','Grupo ficticio para ver catalogos');
INSERT INTO permisos (grupo,clase,permiso) VALUES
    (301,'CClientesView',1),
    (301,'CFormUser',1),
    (301,'CTemplateView',1);
INSERT INTO usuario (id,login,nombre,grupo)
VALUES (401,'consulta_demo','Usuario ficticio',301);
INSERT INTO usuarios (id,login,nombre,descripcion,grupo)
VALUES (401,'consulta_demo','Usuario ficticio','0',301);
# No seeded schedule rows. The original login accepts an unrestricted account with no schedule.
INSERT INTO shared (id,nReservado,nombre,descripcion)
VALUES (501,1,'Categoria demo','Dato ficticio del catalogo auxiliar');
