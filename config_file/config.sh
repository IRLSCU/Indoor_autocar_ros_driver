#!/bin/bash

#set udev rules
sudo bash ./restart_rules.sh

#set frp

sudo apt-get -y install supervisor
sudo cp ./frp.conf /etc/supervisor/conf.d/frp.conf
# 重启supervisor
sudo systemctl restart supervisor
#重启frp服务
sudo supervisorctl restart frp
#sudo cp ./frp_client.sh /etc/init.d/

#chmod 777 /etc/init.d/frp_client.sh
# please edit /etc/rc.local and add next command before exit 0
#/etc/init.d/frp_client.sh start



