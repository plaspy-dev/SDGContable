#!/bin/sh

# The original desktop first logs in with the encoded name of this fictional user.
case "$MYSQL_PASSWORD" in
    ''|*[!0-9a-fA-F]*) printf '%s\n' 'Use the generated hexadecimal demo password.' >&2; exit 1;;
esac
demo_alias=$(printf '%s' consulta_demo | md5sum | cut -c1-15)
MYSQL_PWD="$MYSQL_ROOT_PASSWORD" mysql --user=root <<EOF
CREATE USER '$demo_alias'@'%' IDENTIFIED BY '$MYSQL_PASSWORD';
GRANT SELECT, INSERT, UPDATE, DELETE ON sdgcontable_demo.* TO '$demo_alias'@'%';
EOF
