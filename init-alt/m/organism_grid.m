function [grid] = organism_grid(f, organisms, grid)
  for i = 1:length(organisms)
    grid(organisms(i).x, organisms(i).y) = feval(f, i);
  end
end
