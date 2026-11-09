// MSC-Script //

#strict 3

protected func Hit(...) {
  CastPXS("Rock",600,35);
  CastPXS("Granite",600,35);
  RemoveObject();
  return(1);
}

/* Kann chemisch werden */
func IsConcreteProduct(...) { return(1);
}

public func GetResearchBase(...) { return(PNCR);
}

