function [delta] = multiply_delta(delta, n)
  delta = [ delta(:,1) delta(:,2) n * delta(:,3) ];
end
