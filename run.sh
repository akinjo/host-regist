#!/bin/sh
SQL_LIST='sql/convert.sql sql/insert_admin_user.sql sql/insert_domain.sql'

/usr/bin/php ./convert.php   > sql/convert.sql

cat $SQL_LIST | psql -U named -h 133.13.48.3

