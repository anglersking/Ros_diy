# 首先 first
git clone https://github.com/anglersking/Ros_diy.git
# 然后 then
cd Ros_diy
# 最后 finally
catkin_make
#
# 请开启三个终端
# run publish and advertis please open three Terminal and run

source devel/setup.bash

# 三个终端分别输入
# Three Terminal one、two、three

## one

roscore

## two

rosrun topic_test talker

## three

rosrun topic_test listenner

# 运行服务第一个终端roscore不要关
# run service (sever and client)

## two

rosrun topic_test add_tow_ints_sever

## three client with two par

rosrun topic_test add_tow_ints_client 1 2
