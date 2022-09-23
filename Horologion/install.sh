# ----------------------------------------
# Horologion Sleep Wake Cycle Management Software
# ----------------------------------------

SRC_BINARY=$(dirname $0)/bin
BINARY_NAME=horolog
DST_BINARY=/usr/bin

CONFIG_FILE=horolog.ini
SRC_CONFIG=$(dirname $0)/cfg
DST_CONFIG=/etc

SERVICE_FILE=horolog.service
SERVICE_NAME=horolog.service
SRC_SERVICE_FILE=$(dirname $0)/systemd
DST_SERVICE_FILE=/etc/systemd/system

LOG_FILEPATH=/var/log/horolog.log

echo -e "\e[1m\e[4mHorologion Sleep Wake Cycle Management Software\e[0m\n"

if [ $(id --user) -ne 0 ];
    then echo "Please run as root!"
    exit 1
fi

set -e

install()
{
    cmake -S $(dirname $0) -B ${SRC_BINARY}
    make --jobs=12 -C ${SRC_BINARY}
    cp --verbose ${SRC_BINARY}/${BINARY_NAME} ${DST_BINARY}/
    cp --verbose ${SRC_CONFIG}/${CONFIG_FILE} ${DST_CONFIG}/
    cp --verbose ${SRC_SERVICE_FILE}/${SERVICE_FILE} ${DST_SERVICE_FILE}/
}

uninstall()
{
    rm --force --verbose ${DST_SERVICE_FILE}/${SERVICE_FILE}
    rm --force --verbose ${DST_CONFIG}/${CONFIG_FILE}
    rm --force --verbose ${LOG_FILEPATH}
    rm --force --verbose ${DST_BINARY}/${BINARY_NAME}
}

echo "Select setup type:"
echo "[1] -> Install product"
echo "[2] -> Uninstall product"
echo -n "> "
read SETUP_TYPE
echo

if [ $SETUP_TYPE = 1 ]
then
    install
elif [ $SETUP_TYPE = 2 ]
then
    uninstall
else
    echo "Unrecognized setup option!"
    exit 1
fi
