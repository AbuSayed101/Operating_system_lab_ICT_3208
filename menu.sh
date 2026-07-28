#!/bin/bash

while true
do
    echo "========================="
    echo "      MAIN MENU"
    echo "========================="
    echo "1. Show Current Directory"
    echo "2. Show Date & Time"
    echo "3. List Files"
    echo "4. Create Folder"
    echo "5. Exit"

    echo
    read -p "Enter your choice: " choice

    case $choice in
        1)
            echo "Current Directory:"
            pwd
            ;;
        2)
            echo "Current Date & Time:"
            date
            ;;
        3)
            echo "Files:"
            ls -l
            ;;
        4)
            read -p "Enter folder name: " folder
            mkdir -p "$folder"
            echo "Folder '$folder' created."
            ;;
        5)
            echo "Goodbye!"
            break
            ;;
        *)
            echo "Invalid Choice!"
            ;;
    esac

    echo
    read -p "Press Enter to continue..."
done