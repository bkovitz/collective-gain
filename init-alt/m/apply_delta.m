% delta: 3 columns: [x y delta]
% m: matrix to modify
% x y: coordinates in m of the [0 0] of delta

function [m] = apply_delta(delta, m, x, y)
  % adjust coordinate columns in delta
  delta = in_bounds(delta + ones(size(delta))*diag([x y 0]), m);

  % remove out-of-bounds coordinates
%  delta = delta( find(delta(:,1) >= 1 & delta(:,1) <= size(m,1) ...
%                    & delta(:,2) >= 1 & delta(:,2) <= size(m,2)), : );

  % linear indices into m of elements to change
  m_inds = sub2ind(size(m),delta(:,1),delta(:,2));

  % apply the deltas
  m(m_inds) = m(m_inds) + delta(:,3);
end

