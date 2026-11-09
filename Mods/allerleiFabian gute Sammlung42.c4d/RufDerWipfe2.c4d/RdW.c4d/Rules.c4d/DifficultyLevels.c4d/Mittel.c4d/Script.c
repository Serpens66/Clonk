#strict
#include LGHT

public func GetFood()	{return(2);}
public func GetScienceCost()  {return(25);}
public func GetRevTime()   {return(50);}
public func GetPollutionPerc()  {return(50);}

public func IsDifficultyLevel() {return(_inherited()+1);}
public func Color(){return RGB(64,64,255);}
