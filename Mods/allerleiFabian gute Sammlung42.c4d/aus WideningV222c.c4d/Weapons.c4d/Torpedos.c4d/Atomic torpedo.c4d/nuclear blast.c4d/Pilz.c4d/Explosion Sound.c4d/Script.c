/*--Sound fade out --*/

#strict

protected Initialize:
	SetAction("FADE_OUT");
	return(1);
protected fade_out:
	SetPosition(GetX(),GetY()-Random(2));
	return(1);
nicht_markieren: return(1);