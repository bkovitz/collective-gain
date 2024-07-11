function [delta_inds, target_inds] = in_bounds_indices(delta, target, x, y)
  num_rows = size(delta,1);
  delta = delta + ones(size(delta))*diag([x y 0]);
  ok = delta(:,1) >= 1 & delta(:,1) <= size(target,1) ...
     & delta(:,2) >= 1 & delta(:,2) <= size(target,2);
  d = [zeros(num_rows,2) ok];
  delta_inds = find(d);
  [rows,~] = ind2sub(size(delta), delta_inds);
  target_inds = sub2ind(size(target), delta(rows,1), delta(rows,2));
end
