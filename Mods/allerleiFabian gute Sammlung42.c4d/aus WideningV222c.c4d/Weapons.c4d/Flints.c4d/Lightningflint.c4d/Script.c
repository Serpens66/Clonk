#strict

// MSC-Script //

Hit:
  Sound("Thunder*");
    LaunchLightning(Sum(GetX(),Random(150)),0, -20,35, +4,14 );
    LaunchLightning(Sum(GetX(),Random(150)),0, -20,35, +4,14 );
    LaunchLightning(Sum(GetX(),Random(150)),0, -20,41, +4,14 );
    LaunchLightning(Sum(GetX(),Random(150)),0, -21,41, +5,15 );
    LaunchLightning(Sum(GetX(),Random(150)),0, -21,37, +5,15 );
    RemoveObject();
  return(1);

Activate:
  Sound("Thunder*");
  AssignVar( 0, Sum( GetX(Par(0)), GetVertex(0,0,Par(0)) ) );
  AssignVar( 1, Sum( GetY(Par(0)), GetVertex(0,1,Par(0)) ) );
  AssignVar( 2, Sum( -20, Mul(25,GetDir(Par(0))) ) );
  AssignVar( 3, 15 );
  LaunchLightning( Var(0), Var(1), Var(2), Var(3), -4,-4 );
  LaunchLightning( Var(0), Var(1), Var(2), Var(3), -3,-3 );
  LaunchLightning( Var(0), Var(1), Var(2), Var(3), -2,-2 );
  LaunchLightning( Var(0), Var(1), Var(2), Var(3), -1,-1 );
  LaunchLightning( Var(0), Var(1), Var(2), Var(3), 0,0 );
  LaunchLightning( Var(0), Var(1), Var(2), Var(3), 0,0 );
  LaunchLightning( Var(0), Var(1), Var(2), Var(3), 1,1 );
  LaunchLightning( Var(0), Var(1), Var(2), Var(3), 2,2 );
  LaunchLightning( Var(0), Var(1), Var(2), Var(3), 3,3 );
  LaunchLightning( Var(0), Var(1), Var(2), Var(3), 4,4 );
  RemoveObject();
  return(1);

/* Kann chemisch werden */
IsChemicalProduct: return(1);

/* Forschung */

public GetResearchBase: return(EFLN);


