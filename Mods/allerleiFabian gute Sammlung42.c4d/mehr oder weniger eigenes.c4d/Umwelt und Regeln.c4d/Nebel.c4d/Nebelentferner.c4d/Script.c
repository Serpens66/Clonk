/*-- Nebelentferner --*/

#strict 3


func Initialize()
{
SetPosition();
}

func KillNebel()
{
if ((ObjectCount2(Find_ID(NBL1)) ?? 0)>=1) RemoveAll(NBL1);

if (Equal(ObjectCount2(Find_ID(NB9Z)), nil))
{if ((ObjectCount2(Find_ID(NBL4)) ?? 0)>=1) {RemoveAll(NBL4);} return;}

if (Equal(ObjectCount2(Find_ID(NB9Z)), 6)) //sollten eig maximal 5 sein
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
return;};

if (Equal(ObjectCount2(Find_ID(NB9Z)), 5))
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
return;};

if (Equal(ObjectCount2(Find_ID(NB9Z)), 4))
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
return;};

if (Equal(ObjectCount2(Find_ID(NB9Z)), 3))
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
CreateObject(NB9Z);
return;};

if (Equal(ObjectCount2(Find_ID(NB9Z)), 2))
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
CreateObject(NB9Z);
return;};

if (Equal(ObjectCount2(Find_ID(NB9Z)), 1))
{ClearParticles("Nebel");
ResetGamma();
RemoveAll(NB9Z);
return;};
}

