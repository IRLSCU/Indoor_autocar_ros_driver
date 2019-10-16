#!/usr/bin/python
# -*- coding:utf-8 -*-
import requests
import sys

#登录地址
post_addr="http://192.168.100.101:8080/PortalServer/Webauth/webAuthAction!login.action"

#构造头部信息
post_header={
		'Accept': '*/*',
		'Accept-Encoding': 'gzip, deflate',
		'Accept-Language': 'en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7',
		'Connection': 'keep-alive',
		'Content-Length': '347',
		'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8',
		'Cookie': 'JSESSIONID=CB7F2F6C12FF1B0094548CCF7A105DDD; XSRF-TOKEN=75903e642aacb0fced7e5d67ceeed3d4; SESSIONID_BAK=CB7F2F6C12FF1B0094548CCF7A105DDD',
		'Host': '192.168.100.101:8080',
		'Origin': 'http://192.168.100.101:808',
		'Referer': 'http://192.168.100.101:8080/PortalServer/customize_new/1552379025229/pc/auth.jsp?ssid=FeiXingMoNin&url=http%3A%2F%2Fwww%2Egstat',
		'User-Agent': 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36',
		'X-Requested-With': 'XMLHttpRequest',
		'X-XSRF-TOKEN':'75903e642aacb0fced7e5d67ceeed3d4'
}
post_data2={
	'userName': '2018223045213W',
	'password':'wang1388.',
	'validCode':sys.argv[1],
	'valideCodeFlag':'true',
	'authLan':'zh_CN',
	'hasValidateNextUpdatePassword': 'true',
	'rememberPwd': 'false',
	'browserFlag':'zh',
	'hasCheckCode':'false',
	'checkcode':'',
	'hasRsaToken':'false',
	'rsaToken':'',
	'autoLogin':'false',
	'userMac':'',
	'isBoardPage':'false',
	'disablePortalMac':'false',
	'overdueHour':'0',
	'overdueMinute':'0',
	'isAccountMsgAuth':'',
	'validCodeForAuth':'',
}

response = requests.post(post_addr, data=post_data2, headers=post_header)
print(response)
