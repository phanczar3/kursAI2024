#!/bin/bash

echo "Press ENTER after each test case"

for task_file in task*txt
do
    python3 show_optimal_policy.py $task_file
done
