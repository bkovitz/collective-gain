% Returns rows in coords whose first two columns are x,y coordinates within
% the size of target.

function [coords] = in_bounds(coords, target)
  coords = coords( find(coords(:,1) >= 1 & coords(:,1) <= size(target,1) ...
                      & coords(:,2) >= 1 & coords(:,2) <= size(target,2)), : );
end
