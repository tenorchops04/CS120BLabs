# Test file for "Lab2_introToAVR"
#
#
# # commands.gdb provides the following functions for ease:
# #   test "<message>"
# #       Where <message> is the message to print. Must call this at the beginning of every test
# #       Example: test "PINA: 0x00 => expect PORTC: 0x01"
# #   checkResult
# #       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
# #       Must call this at the end of every test.
# #   expectPORTx <val>
# #       With x as the port (A,B,C,D)
# #       The value the port is epected to have. If not it will print the erroneous actual value
# #   setPINx <val>
# #       With x as the port or pin (A,B,C,D)
# #       The value to set the pin to (can be decimal or hexidecimal
# #       Example: setPINA 0x01
# #   printPORTx f OR printPINx f 
# #       With x as the port or pin (A,B,C,D)
# #       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
# #       Example: printPORTC d
# #   printDDRx
# #       With x as the DDR (A,B,C,D)
# #       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

test "PINA: 0x32, PORTB: 0x32, PORTC: 0x1E => PORTD: 0x80"
setPINA 0x32
setPINB 0x32
setPINC 0x1E
continue 5
expectPORTD 0x80
checkResult

test "PINA: 0x55, PORTB: 0x02, PORTC: 0x03 => PORTD: 0x5A"
setPINA 0x55
setPINB 0x02
setPINC 0x03
continue 5
expectPORTD 0x5A
checkResult

test "PINA: 0x00, PORTB: 0x00, PORTC: 0x00 => PORTD: 0x00"
setPINA 0x00
setPINB 0x00
setPINC 0x00
continue 5
expectPORTD 0x00
checkResult

test "PINA: 0x20, PORTB: 0x20, PORTC: 0x20 => PORTD: 0x60"
setPINA 0x20
setPINB 0x20
setPINC 0x20
continue 5
expectPORTD 0x60
checkResult

test "PINA: 0x04, PORTB: 0x04, PORTC: 0x04 => PORTD: 0x0C"
setPINA 0x04
setPINB 0x04
setPINC 0x04
continue 5
expectPORTD 0x0C
checkResult

# Report on how many tests passed/tests ran
 set $passed=$tests-$failed
 eval "shell echo Passed %d/%d tests.\n",$passed,$tests
 echo ======================================================\n
