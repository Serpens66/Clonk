/*-- Neues Objekt --*/

#strict 3
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
 SetPosition((GetX(parent) ?? 0)-215+430*(GetDir() ?? 0),(GetY(parent) ?? 0)+6);
 return(1);
}