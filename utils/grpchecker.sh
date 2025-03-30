#! /bin/bash

GROUP_NAME="GORGONA_SESSION"
JOB_VALUE=

check_processes( ) {
  cat "/proc/${1}/environ" 2>/dev/null | tr '\0' '\n' | awk -v GN="${2}" -F"=" '$1==GN {print$2}'
}

### Arguments processing ###
while [[ $# -gt 0 ]]; do
  key="$1"
  case "$key" in
   -n|--name ) # Option for check group name
   shift
   GROUP_NAME="$1"
   ;;
  esac
  shift
done

### MAIN ###
for PID in $( pgrep -U "$USER" ); do
  JOB_VALUE="$(check_processes $PID $GROUP_NAME )"
  if [ -n "$JOB_VALUE" ]; then
    printf "%d\n" $PID
  fi
done

exit 0
### END ###
