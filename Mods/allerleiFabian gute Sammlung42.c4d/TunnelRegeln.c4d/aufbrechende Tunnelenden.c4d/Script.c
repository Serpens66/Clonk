#strict 3

static TunnelRules_iBOShakeFree; //todo 11
static TunnelRules_iBOSearchSize; //4


protected func Initialize()
{
TunnelRules_iBreakOpen = 1;
TunnelRules_iBOShakeFree = TunnelRules_getBreakOpenShakeFree();
TunnelRules_iBOSearchSize = TunnelRules_getBreakOpenSearchSize();
}

//folgende Einstellungen können im SzenarioScript überladen werden
//es wird jedoch empfohlen diese hier beizubehalten

global func TunnelRules_getBreakOpenShakeFree() { return(11); } 
global func TunnelRules_getBreakOpenSearchSize() { return(1); }

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}