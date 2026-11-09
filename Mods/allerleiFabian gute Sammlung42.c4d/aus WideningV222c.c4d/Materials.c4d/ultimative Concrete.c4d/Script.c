// MSC-Script //

#strict

protected Hit:
  CastPXS("Rock",600,35);
  CastPXS("Granite",600,35);
  RemoveObject();
  return(1);

/* Kann chemisch werden */
IsConcreteProduct: return(1);

public GetResearchBase: return(PNCR);

