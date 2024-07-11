function [delta_matrix] = make_delta(radius, f)
  delta_matrix = [];
  for x = -radius:radius
    for y = -radius:radius
      delta_matrix = [delta_matrix; [x y f(x,y)]];
    end
  end

  % only rows with non-zero third column
  delta_matrix = delta_matrix(find(delta_matrix(:,3) > 0),:);

  % normalize so sum of all deltas = 1.0
  %scale_delta = diag([1 1 1/sum(delta_matrix(:,3))]);
  %delta_matrix = delta_matrix * scale_delta;
end
