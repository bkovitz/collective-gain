function [t] = a
  function [delta_matrix] = mk(radius, f)
    delta_matrix = [];
    for x = -radius:radius
      for y = -radius:radius
        delta_matrix = [delta_matrix; [x y f(x,y)]];
      end
    end
    % only rows with non-zero third column
    delta_matrix = delta_matrix(find(delta_matrix(:,3) > 0),:);
  end

  function [m] = apply_delta(delta, m, x, y)
    % adjust coordinate columns in delta
    delta = delta + ones(size(delta))*diag([x y 0]);

    % remove out-of-bounds coordinates
    delta = delta( find(delta(:,1) >= 1 & delta(:,1) <= size(m,1) ...
                      & delta(:,2) >= 1 & delta(:,2) <= size(m,2)), : );

    % linear indices into m of elements to change
    m_inds = sub2ind(size(m),delta(:,1),delta(:,2));

    % apply the deltas
    m(m_inds) = m(m_inds) + delta(:,3);
  end

  a_rad = 4;  % absorption radius
  d = mk(4, @(x,y)dist([x y],[0 0]') <= a_rad);
  t = zeros(10,10);
  t = apply_delta(d,t,1,5)
  t = apply_delta(d,t,5,5)
end
