% "collective gain" simulation

function cg
  world_size = [10 10];
  a_rad = 4;   % absorption radius
  max_absorption = 40;
  num_initial_organisms = 20;
  num_generations = 1;

  absorption_delta = make_delta(a_rad, @(x,y)a_rad-dist([x y],[0 0]')) ...
                     * diag([1 1 max_absorption]);


  % populate world
  olocs = zeros(world_size);
  organisms = [];

  for i = 1:num_initial_organisms
    possible_locs = in_bounds(random('unid', 10, 60, 2), olocs)
    for attempt = 1:size(possible_locs,1)
      loc = possible_locs(attempt,:);
      x = loc(1); y = loc(2);
      if ~olocs(x, y)
        olocs(x, y) = 1;
        break
      end
    end
    o.g = 0;
    o.x = x;
    o.y = y;
    o.absorbed = 0;
    o.giver = 0;
    organisms = [organisms o];
  end

  % main loop
  for gen = 1:num_generations

  %   all organisms try to absorb as much sunlight as they can
    t = zeros(world_size);
    for i = 1:length(organisms)
      o = organisms(i);
      t = apply_delta(absorption_delta, t, o.x, o.y);
    end

    olocs
    sum(sum(olocs))
    t

  %   actual absorption: share sunlight in contested cels
  %   decide who gives and who takes
  %   givers create protection zones
  %   predation
  %   create next generation

  end


%  attempted_absorption = zeros(world_size);
%
%  attempted_absorption = apply_delta( ...
%      absorption_delta, attempted_absorption, 5, 5);
%  attempted_absorption = apply_delta( ...
%      absorption_delta, attempted_absorption, 9, 10);



%  organisms = [];
%  for i = 1:num_initial_organisms
%    o.g = 0;
%    o.absorbed = 0;
%    [loc,olocs] = unoccupied_location(@()random('unid', world_size), olocs);
%    o.x = loc(1);
%    o.y = loc(2);
%    organisms = [organisms o];
%  end
%  olocs
%  sum(sum(olocs))
%  organisms
%
%  function [loc,olocs] = unoccupied_location(f, olocs)
%    while 1
%      loc = f();
%      break
%      if ~olocs(loc(1), loc(2))
%        break
%      end
%    end
%    olocs(loc(1), loc(2)) = 1;
%    %throw(MException('occ:x', 'x'))
%  end
  
end
