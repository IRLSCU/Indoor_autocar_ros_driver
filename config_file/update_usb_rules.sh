#!/bin/bash

RULES_PATH="/etc/udev/rules.d/"
NEW_RULE_NAME="com_port.rules"
FULL_PATH=$RULES_PATH$NEW_RULE_NAME
echo $FULL_PATH
#检测文件是否存在

if [ -f "$FULL_PATH" ];then
	sudo rm -rf $FULL_PATH
	echo "removed file"
fi
sudo cp $NEW_RULE_NAME  $FULL_PATH
sudo chmod 666 $FULL_PATH
echo "cpoy file finish"
# restart udev
sudo service udev reload
sudo service udev restart
