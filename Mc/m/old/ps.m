function ps
  world_size = [10 10];
  a_rad = 4;   % absorption radius
  max_absorption = 20;
  num_initial_organisms = 10;

  absorption_delta = max_absorption * make_delta_matrix( ...
      a_rad, @(x,y)dist([x y],[0 0]') <= a_rad);


  attempted_absorption = zeros(world_size);

  attempted_absorption = apply_delta( ...
      absorption_delta, attempted_absorption, 9, 10);
  attempted_absorption = apply_delta( ...
      absorption_delta, attempted_absorption, 5, 5);

  olocs = zeros(world_size);
  organisms = [];
  for i = 1:num_initial_organisms
    o.g = 0;
    o.absorbed = 0;
    [loc,olocs] = unoccupied_location(@()random('unid', world_size), olocs);
    o.x = loc(1);
    o.y = loc(2);
    organisms = [organisms o];
  end
  olocs
  sum(sum(olocs))
  organisms

  function [loc,olocs] = unoccupied_location(f, olocs)
    while 1
      loc = f();
      break
      if ~olocs(loc(1), loc(2))
        break
      end
    end
    olocs(loc(1), loc(2)) = 1;
    %throw(MException('occ:x', 'x'))
  end
  
end
