function [x, y, olocs] = find_unoccupied(possibles, olocs)
  possibles = in_bounds(possibles, olocs);
  for i = 1:size(possibles,1)
    x = possibles(i,1);
    y = possibles(i,2);
    if ~olocs(x,y)
      olocs(x,y) = 1;
      return
    end
  end
  x = -1;  % indicate that all possible locations were occupied
  y = -1;
end
