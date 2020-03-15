#!/usr/bin/expect

#*** Precision Microdrives [PMD] ***
#* Haptic Feedback Evaluation Kit 2.0 [HFEK2]
#- Tristan Cool
#- July 2109

#***************************************************#
#    This script is automatic firmware update.      #
#***************************************************#

echo "............. -PMD HFEK2- .................."
echo "update_auto.bash"

set user "pmd-hfek2"
set password "PMD888dev"

spawn ./update.bash

expect "Username for 'https://github.com':"
send "$user\r"
expect "Password for 'https://pmd-hfek2@github.com':"
send "$password\r"

expect eof
