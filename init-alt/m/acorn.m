function [xy_rows] = acorn(center_x, center_y, sigma)
  num_rows = 200;  % number of possible tries to place the acorn
  d = abs(normrnd(0, sigma, num_rows, 1));
  angle = unifrnd(0, 2*pi, num_rows, 1);
  [x,y] = pol2cart(angle, d);
  xy_rows = round([x+center_x y+center_y]);
end
