# ----------------------------------------
# Horologion Sleep Wake Cycle Management Software
# ----------------------------------------

SRC_BINARY=$(dirname $0)/bin
BINARY_NAME=horolog
DST_BINARY=/usr/bin

CONFIG_FILE=horolog.ini
SRC_CONFIG=$(dirname $0)/cfg
DST_CONFIG=/etc

echo -e "\e[1m\e[4mHorologion Sleep Wake Cycle Management Software\e[0m\n"

if [ $(id --user) -ne 0 ];
    then echo "Please run as root!"
    exit 1
fi

compile_binary()
{
    echo "Compiling binary"

    cmake -S $(dirname $0) -B ${SRC_BINARY} && make --jobs=12 -C ${SRC_BINARY}

    if [ $? -ne 0 ];
        then exit 1
    fi
}

copy_binary()
{
    echo "Copying ${BINARY_NAME} binary to ${DST_BINARY}/"

    cp --verbose ${SRC_BINARY}/${BINARY_NAME} ${DST_BINARY}/

    if [ $? -ne 0 ];
        then exit 1
    fi
}

remove_binary()
{
    echo "Removing ${BINARY_NAME} binary from ${DST_BINARY}/"

    rm --verbose ${DST_BINARY}/${BINARY_NAME}

    if [ $? -ne 0 ];
        then exit 1
    fi
}

copy_config_file()
{
    echo "Copying ${CONFIG_FILE} to ${DST_CONFIG}/"

    cp --verbose ${SRC_CONFIG}/${CONFIG_FILE} ${DST_CONFIG}/

    if [ $? -ne 0 ];
        then exit 1
    fi
}

remove_config_file()
{
    echo "Removing ${CONFIG_FILE} from ${DST_CONFIG}/"

    rm --verbose ${DST_CONFIG}/${CONFIG_FILE}

    if [ $? -ne 0 ];
        then exit 1
    fi
}

install()
{
    compile_binary
    copy_binary
    copy_config_file
}

uninstall()
{
    remove_config_file
    remove_binary
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
