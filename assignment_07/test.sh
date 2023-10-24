#!/bin/bash

rm -f log.txt

print_yellow() {
  echo -e "\e[93m$1\e[0m"
}

print_green() {
  echo -e "\e[32m$1\e[0m"
}

print_red() {
  echo -e "\e[31m$1\e[0m"
}

passed_tests=0

# Test A
print_yellow "Running Test A..."
if (cat /dev/chdev-0 > log.txt &) && killall cat; then
  expected_output="[ 1050.623810] Aaltodriver: Reading device: 0\n[ 1050.624914] Aaltodriver: CHDEV: Device close"
  actual_output=$(sudo dmesg | tail -n 2)
  if [ "$(echo "$actual_output" | sed 's/\[[^]]*\]\s*//g')" = "$(echo -e "$expected_output" | sed 's/\[[^]]*\]\s*//g')" ]; then
    print_green "Test A Passed"
    passed_tests=$((passed_tests + 1))
  else
    print_red "Test A Failed: Unexpected kernel output"
  fi
else
  print_red "Test A Failed: Command execution failed"
fi

# Test B
print_yellow "Running Test B..."
if (cat /dev/chdev-1 > log.txt &) && killall cat; then
  expected_output="[ 1050.623810] Aaltodriver: Reading device: 1\n[ 1050.624914] Aaltodriver: CHDEV: Device close"
  actual_output=$(sudo dmesg | tail -n 2)
  if [ "$(echo "$actual_output" | sed 's/\[[^]]*\]\s*//g')" = "$(echo -e "$expected_output" | sed 's/\[[^]]*\]\s*//g')" ]; then
    print_green "Test B Passed"
    passed_tests=$((passed_tests + 1))
  else
    print_red "Test B Failed: Unexpected kernel output"
  fi
else
  print_red "Test B Failed: Command execution failed"
fi

# Test C
print_yellow "Running Test C..."
if (cat /dev/chdev-0 > log.txt &) && killall cat; then
  sleep 1
  echo "hello" > /dev/chdev-0
  expected_output="[ 2598.095853] Aaltodriver: Reading device: 0\n[ 2598.104624] Aaltodriver: CHDEV: Device close\n[ 2627.861126] Aaltodriver: CHDEV: Device open\n[ 2627.861149] Aaltodriver: Writing device: 0\n[ 2627.861152] Aaltodriver: Copied 6 bytes from the user\n[ 2627.861154] Aaltodriver: Data from the user: hello\n\n[ 2627.861243] Aaltodriver: CHDEV: Device close"
  actual_output=$(sudo dmesg | tail -n 8)
  if [ "$(echo "$actual_output" | sed 's/\[[^]]*\]\s*//g')" = "$(echo -e "$expected_output" | sed 's/\[[^]]*\]\s*//g')" ]; then
    print_green "Test C Passed"
    passed_tests=$((passed_tests + 1))
  else
    print_red "Test C Failed: Unexpected kernel output"
  fi
else
  print_red "Test C Failed: Command execution failed"
fi


print_yellow "Running Test D..."
if (cat /dev/chdev-1 > log.txt &) && killall cat; then
  sleep 1
  echo "hello" > /dev/chdev-1
  expected_output="[ 2598.095853] Aaltodriver: Reading device: 1\n[ 2598.104624] Aaltodriver: CHDEV: Device close\n[ 2627.861126] Aaltodriver: CHDEV: Device open\n[ 2627.861149] Aaltodriver: Writing device: 1\n[ 2627.861152] Aaltodriver: Copied 6 bytes from the user\n[ 2627.861154] Aaltodriver: Data from the user: hello\n\n[ 2627.861243] Aaltodriver: CHDEV: Device close"
  actual_output=$(sudo dmesg | tail -n 8)
  if [ "$(echo "$actual_output" | sed 's/\[[^]]*\]\s*//g')" = "$(echo -e "$expected_output" | sed 's/\[[^]]*\]\s*//g')" ]; then
    print_green "Test D Passed"
    passed_tests=$((passed_tests + 1))
  else
    print_red "Test D Failed: Unexpected kernel output"
  fi
else
  print_red "Test D Failed: Command execution failed"
fi

# Test E
print_yellow "Running Test E..."
if (cat /dev/chdev-0 > log.txt &) && killall cat; then
  sleep 1
  echo "hellooooooooooooooooooooooo" > /dev/chdev-0
  expected_output="[ 3352.225292] Aaltodriver: Reading device: 0\n[ 3352.232927] Aaltodriver: CHDEV: Device close\n[ 3356.258400] Aaltodriver: CHDEV: Device open\n[ 3356.258413] Aaltodriver: Writing device: 0\n[ 3356.258414] Aaltodriver: Copied 15 bytes from the user\n[ 3356.258415] Aaltodriver: Data from the user: hellooooooooooo\n[ 3356.258433] Aaltodriver: CHDEV: Device close"
  actual_output=$(sudo dmesg | tail -n 7)
  if [ "$(echo "$actual_output" | sed 's/\[[^]]*\]\s*//g')" = "$(echo -e "$expected_output" | sed 's/\[[^]]*\]\s*//g')" ]; then
    print_green "Test E Passed"
    passed_tests=$((passed_tests + 1))
  else
    print_red "Test E Failed: Unexpected kernel output"
  fi
else
  print_red "Test E Failed: Command execution failed"
fi


# Test F
print_yellow "Running Test F..."
if (cat /dev/chdev-1 > log.txt &) && killall cat; then
  sleep 1
  echo "hellooooooooooooooooooooooo" > /dev/chdev-1
  expected_output="[ 3352.225292] Aaltodriver: Reading device: 1\n[ 3352.232927] Aaltodriver: CHDEV: Device close\n[ 3356.258400] Aaltodriver: CHDEV: Device open\n[ 3356.258413] Aaltodriver: Writing device: 1\n[ 3356.258414] Aaltodriver: Copied 15 bytes from the user\n[ 3356.258415] Aaltodriver: Data from the user: hellooooooooooo\n[ 3356.258433] Aaltodriver: CHDEV: Device close"
  actual_output=$(sudo dmesg | tail -n 7)
  if [ "$(echo "$actual_output" | sed 's/\[[^]]*\]\s*//g')" = "$(echo -e "$expected_output" | sed 's/\[[^]]*\]\s*//g')" ]; then
    print_green "Test F Passed"
    passed_tests=$((passed_tests + 1))
  else
    print_red "Test F Failed: Unexpected kernel output"
  fi
else
  print_red "Test F Failed: Command execution failed"
fi

print_yellow "Number of passed tests: $passed_tests out of 6 tests"