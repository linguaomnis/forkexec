#!/bin/bash

LIZM_RET_OK=0;
LIZM_RET_NG=1;
LIZM_PROGNAME=$(basename $0);
LIZM_DAEMON_ID="__LIZM__";
LIZM_DAEMON_FNC_START="";
LIZM_DAEMON_FNC_LOOP="";
LIZM_DAEMON_FNC_STOP="";
LIZM_WAIT_ORA=0.1;
LIZM_WAIT_VOLTA=300;

function _lizm_daemon_usage ()
{
cat <<DOC
${LIZM_PROGNAME} <status|start|stop>
DOC
}

function _lizm_daemon_status ()
{
    local RET=${LIZM_RET_OK};

    pgrep -f ${LIZM_DAEMON_ID} 2>&1 >/dev/null;

    if [[ $? == 0 ]]; then
        echo "${LIZM_PROGNAME} is running.";
    else
        echo "${LIZM_PROGNAME} is not running.";
    fi  
    
    return ${RET};
}

function _lizm_daemon_start ()
{
    local RET=0;
    local STAT=0;

    pgrep -f ${LIZM_DAEMON_ID} 2>&1 >/dev/null;
    STAT=$?;

    if [[ ${STAT} != 0 ]]; then

        printf "start ${LIZM_PROGNAME} ... ";

        (exec nohup $0 ${LIZM_DAEMON_ID} </dev/null 2>&1 >/dev/null) \
             </dev/null 2>&1 >/dev/null &

        for INDEX in $(seq 1 ${LIZM_WAIT_VOLTA}); do

            pgrep -f ${LIZM_DAEMON_ID} 2>&1 >/dev/null;
            STAT=$?;

            if [[ ${STAT} == 0 ]]; then
                break;
            fi

            sleep ${LIZM_WAIT_ORA};

        done;

        if [[ ${STAT} == 0 ]]; then
            echo "OK.";
            RET=0;
        else
            echo "NG.";
            RET=1;
        fi
    else
        echo "${LIZM_PROGNAME} is already running.";
    fi

    return ${RET};
}

function _lizm_daemon_stop ()
{
    local RET=0;
    local STAT=0;

    pgrep -f ${LIZM_DAEMON_ID} 2>&1 >/dev/null;
    STAT=$?;

    if [[ ${STAT} == 0 ]]; then

        printf "stop ${LIZM_PROGNAME} ... ";
        pkill -f ${LIZM_DAEMON_ID};

        for INDEX in $(seq 1 ${LIZM_WAIT_VOLTA}); do

            pgrep -f ${LIZM_DAEMON_ID} 2>&1 >/dev/null;
            STAT=$?;

            if [[ ${STAT} != 0 ]]; then
                break;
            fi

            sleep ${LIZM_WAIT_ORA};

        done;

        if [[ ${STAT} != 0 ]]; then
            echo "OK.";
            RET=0;
        else
            echo "NG.";
            RET=1;
        fi
    else
        echo "${LIZM_PROGNAME} is already not running.";
    fi

    return ${RET};
}

function _lizm_daemon_sigterm ()
{
    local RET=${LIZM_RET_NG};

    ${LIZM_DAEMON_FNC_STOP};
    RET=$?;

    # return ${RET};
    exit ${RET};
}

function _lizm_daemon_loop ()
{
    local RET=${LIZM_RET_NG};

    trap _lizm_daemon_sigterm SIGTERM;

    ${LIZM_DAEMON_FNC_START};

    while true; do
        ${LIZM_DAEMON_FNC_LOOP};
        RET=$?;
    done;

    return ${RET};
}


################################################################################
# lizm_daemon <id> <start cmd/fnc> <loop cmd/fnc> <stop cmd/fnc> [args ...]
################################################################################
function lizm_daemon ()
{
    local RET=${LIZM_RET_NG};
    LIZM_DAEMON_ID=$1; shift;
    LIZM_DAEMON_FNC_START=$1; shift;
    LIZM_DAEMON_FNC_LOOP=$1; shift;
    LIZM_DAEMON_FNC_STOP=$1; shift;
    local ARG=$@;
    local RET=${LIZM_RET_NG};
    local CMD=$1;

    type ${LIZM_DAEMON_FNC_LOOP} 2>&1 >/dev/null;
    if [[ $? != 0 ]]; then
        echo "error : invalid loop procedure : ${LIZM_DAEMON_FNC_LOOP}" >&2;
        exit ${LIZM_RET_NG};
    fi

    type ${LIZM_DAEMON_FNC_STOP} 2>&1 >/dev/null;
    if [[ $? != 0 ]]; then
        echo "error : invalid stop procedure : ${LIZM_DAEMON_FNC_STOP}" >&2;
        exit ${LIZM_RET_NG};
    fi

    case ${CMD} in
    'status')
        _lizm_daemon_status ${LIZM_DAEMON_ID};
        RET=$?;
        ;;
    'start')
        _lizm_daemon_start ${LIZM_DAEMON_ID};
        RET=$?;
        ;;
    'stop')
        _lizm_daemon_stop ${LIZM_DAEMON_ID};
        RET=$?;
        ;;
    ${LIZM_DAEMON_ID})
        _lizm_daemon_loop;
        RET=$?;
        ;;
    *)
        _lizm_daemon_usage;
        RET=${LIZM_RET_NG};
        ;;
    esac

    exit ${RET};
}




