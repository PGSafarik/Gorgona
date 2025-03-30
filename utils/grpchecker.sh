#! /bin/bash
####################################################################################################
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
####################################################################################################
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
