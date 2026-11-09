#strict
#include HARD

public func GetFood()	{return(5);}
public func GetScienceCost()  {return(150);}
public func GetRevTime()   {return(200);}
public func GetPollutionPerc()  {return(200);}

public func IsDifficultyLevel() {return(_inherited()+1);}
public func Color(){return RGB(255,0,0);}
