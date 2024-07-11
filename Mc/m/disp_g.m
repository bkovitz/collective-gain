function [string] = disp_g(g)
  if g < 0
    string = '    -';
  else
    string = sprintf('%5.0f', g*100);
  end
end

