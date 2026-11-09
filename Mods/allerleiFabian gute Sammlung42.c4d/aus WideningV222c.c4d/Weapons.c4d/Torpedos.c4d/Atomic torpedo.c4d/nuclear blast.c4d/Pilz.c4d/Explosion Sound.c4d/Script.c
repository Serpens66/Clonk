/*--Sound fade out --*/

#strict 3

protected func Initialize(...) {
	SetAction("FADE_OUT");
	return(1);
}
protected func fade_out(...) {
	SetPosition(GetX(),(GetY() ?? 0)-(Random(2) ?? 0));
	return(1);
}
func nicht_markieren(...) { return(1);
}