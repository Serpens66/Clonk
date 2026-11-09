#strict 3
#include NORM

public func GetFood()	{return(4);}
public func GetScienceCost()  {return(100);}
public func GetRevTime()   {return(150);}
public func GetPollutionPerc()  {return(150);}

public func IsDifficultyLevel() {return((_inherited() ?? 0)+1);}
public func Color(){return RGB(127,nil,255);}
