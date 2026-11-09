#strict 3
#include FLGR

public func GetFood()	{return(3);}
public func GetScienceCost()  {return(50);}
public func GetRevTime()   {return(100);}
public func GetPollutionPerc()  {return(100);}

public func IsDifficultyLevel() {return((_inherited() ?? 0)+1);}
public func Color(){return RGB(nil,nil,255);}
