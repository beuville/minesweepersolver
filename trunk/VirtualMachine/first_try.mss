#pick squares sequentially starting at 0,0
int xpos=0
int ypos=0
while xpos<9
while ypos<9
picsq xpos,ypos
update ypos=ypos+1
ewhile
update ypos=0
update xpos=xpos+1
ewhile
