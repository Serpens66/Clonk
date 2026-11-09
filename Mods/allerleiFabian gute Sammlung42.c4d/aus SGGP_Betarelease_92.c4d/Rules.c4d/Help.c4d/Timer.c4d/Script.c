#strict 3
local iMinute,iSecound,TimeOver,Disabled,Count;
func Initialize() 
{
	TimeOver = nil;
	SetVisibility(VIS_All);
  	SetPosition(50, 100, this());
        return;
}

func Timer() 
{
	if(Equal(Count, nil)) Count = -1;
	if(!Disabled) 
	{
		S42MigrationAssign(iSecound, "+=", (Count ?? 0));
		Hexal();
		if((Count ?? 0) < 0) Message("<c ff0000>%d:%3.2d</c>", this(),iMinute,iSecound);
		if((iMinute ?? 0) < 60) if((Count ?? 0) > 0) Message("<c 00ff00>%d:%3.2d</c>", this(),iMinute,iSecound);
		if((iMinute ?? 0) > 59) if((Count ?? 0) > 0) Message("<c cc00ff>Süchtig!</c>",this());
	}
}

global func ChangeTimerCount(int iC)
{
	iC ??= 0;
	CheckHelp();
	var pTimer;
	pTimer = FindObject(TIMR);
	if(!pTimer) pTimer = CreateObject(TIMR);
	LocalN("Count",pTimer) = iC;
}

func Hexal()
{
	if(Or((iSecound ?? 0) < 0, (iSecound ?? 0) > 59))
	{
		S42MigrationAssign(iMinute, "+=", (Count ?? 0));
		if((Count ?? 0) > 0) iSecound = nil;
		if((Count ?? 0) < 0) iSecound = 60;
	}
	if((iMinute ?? 0) < 0)
	{
		if((Count ?? 0) < 0) Down();
		iMinute = nil;
	}
	return;
}
	
func Enable()
{
	Disabled = nil;
}

global func SetTime(int iMinutes,int iSecounds)
{
	iMinutes ??= 0;
	iSecounds ??= 0;
	CheckHelp();
	var pTimer;
	pTimer = FindObject(TIMR);
	if(!pTimer) pTimer = CreateObject(TIMR);
	LocalN("iMinute",pTimer) = iMinutes;
	LocalN("iSecound",pTimer) = iSecounds;
	pTimer -> Enable();
	return(1);
}

func Down()
{
	Disabled = 1;
	GameCallEx("TimerDown", this());
}

global func CheckHelp()
{
	if(!FindObject(HELP)) CreateObject(HELP);
}