function [delta_matrix] = make_delta_matrix(radius, f)
  for x = -radius:radius
    for y = -radius:radius
      delta_matrix(x+radius+1,y+radius+1) = f(x,y);
    end
  end
  delta_matrix = delta_matrix .* 1/sum(sum(delta_matrix));
end
