#!/bin/bash


FRP_DIR=home/nvidia/frp_0.27.1_linux_arm64
command=""
config_option=""


help_info(){
	echo "------ frp help info start -------";
	echo ""
	echo "Usage: "
	echo "This sh is tool to start frp fast with nohup "
	echo "Example: ./start_frp_client -c start "
	echo "option [h|c|i] "
	echo " -h:	Show the help information "
	echo " -i:	Show the var and recent log "
	echo " "
	echo "option -i [config|log] default:config "
	echo "		config:	 	Show config file detail "
	echo "		log: 	 	Show the recent log "
	echo " "
	echo "option -c [start|stop|restart] "
	echo " 		start:		start this frp "
	echo "		stop:		stop frp client "
	echo "		restart:  	restart frp client "
	echo ""
	echo "------ frp help info end   ------- ";

}
start_frp(){
	echo "------ start frp info start ------ ";
	cd $FRP_DIR;
	nohup ./frpc -c ./frpc.ini >> frpc.log 2>&1 &
	str=$"\n"
	sstr=$(echo -e $str)
	echo $sstr
	tail 10 frpc.log
	
	echo "------ start frp info end ------ ";
}
get_state(){
	echo "------ get state start ------"
	cd $FRP_DIR
	tail 10 frpc.log
	echo "------ get state end   ------"
}
kill_frp()
{
	echo "------ kill frp start  ------ ";
	kill -9 $(ps -ef | grep frp | grep -v grep | awk '{print $2}')
	echo "------ kill frp end   ------- ";
}


	case "$1" in 
	    "help")
	      help_info
	     ;;
	    "start")
	      start_frp
	     ;;
	    "stop")
	       echo "stop"
	       kill_frp
	     ;;
	    "restart")
	      kill_frp
	      start_frp
	     ;;
	    *)
	      start_frp
	    ;;
	esac
exit 0
