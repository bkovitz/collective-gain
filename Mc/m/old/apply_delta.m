function [m] = apply_delta(delta_matrix, m, x, y)
  dxrad = (size(delta_matrix, 1)-1) / 2;
  dyrad = (size(delta_matrix, 2)-1) / 2;
  %mxlb = max(1, x-dxrad)
  %mxub = min(size(m,1), x+dxrad)

  mxlb = x-dxrad;
  dxlb = 1;
  if mxlb < 1
    xskip = 1 - mxlb;
    mxlb = mxlb + xskip;
    dxlb = dxlb + xskip;
  end

  mylb = y-dyrad;
  dylb = 1;
  if mylb < 1
    yskip = 1 - mylb;
    mylb = mylb + yskip;
    dylb = dylb + yskip;
  end

  mxub = x+dxrad;
  dxub = size(delta_matrix,1);
  if mxub > size(m,1)
    xskip = mxub - size(m,1);
    mxub = mxub - xskip;
    dxub = dxub - xskip;
  end

  myub = y+dyrad;
  dyub = size(delta_matrix,2);
  if myub > size(m,2)
    yskip = myub - size(m,2);
    myub = myub - yskip;
    dyub = dyub - yskip;
  end

  m(mxlb:mxub,mylb:myub) = ...
      m(mxlb:mxub,mylb:myub) + delta_matrix(dxlb:dxub,dylb:dyub);
end

