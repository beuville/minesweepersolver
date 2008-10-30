#mss code to output 20 fibbonocci numbers
int current=1
int previous=1
int temp=0
int i=0
while i<20
disp current
update temp=current
update current=current+previous
update previous=temp
update i=i+1
ewhile
