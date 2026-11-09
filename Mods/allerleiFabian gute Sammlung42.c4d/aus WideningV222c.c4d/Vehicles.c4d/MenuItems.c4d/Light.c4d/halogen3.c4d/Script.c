/*-- Neues Objekt --*/

#strict
local parent;
local old_dir;
#include LICH


func set_parent(creator)
{
 parent=creator;
 SetAction("Shine");
 return(1);
}

func timer_call() //´s gat it deshalb moch i´s nui
{
 if(!parent) RemoveObject();
 SetDir(GetDir(parent));
 SetPosition(GetX(parent)-215+430*GetDir(),GetY(parent)+6);
 return(1);
}