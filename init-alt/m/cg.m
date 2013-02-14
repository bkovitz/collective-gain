% "collective gain" simulation

function cg
  world_size = [10 10];
  a_rad = 4;   % absorption radius
  protection_factor = 1;
  predation_factor = 0.9;
  max_absorption = 40;
  num_initial_organisms = 20;
  num_generations = 1;

  absorption_delta = make_delta(a_rad, @(x,y)a_rad-dist([x y],[0 0]'));
                     %* diag([1 1 max_absorption]);
  protection_delta = multiply_delta(absorption_delta, protection_factor);

  make_initial_population()

  for gen = 1:num_generations

    % all organisms try to absorb as much sunlight as they can
    t = zeros(world_size);
    for i = 1:length(organisms)
      o = organisms(i);
      t = apply_delta(absorption_delta, t, o.x, o.y);
    end

    % actual absorption: share sunlight in contested cells
    t = max(t, ones(size(t)));
    absorbed = arrayfun(@(o)share_sunlight(o,t), organisms)

    % decide who gives and who takes
    givers = arrayfun(@(o)unifrnd(0,1) < o.g, organisms);

    % givers create protection zones
    protection = ones(world_size);
    if any(givers)
      giver_inds = find(givers);
      for i = 1:length(giver_inds)
        o = organisms(giver_inds(i));
        delta = multiply_delta(protection_delta, absorbed(giver_inds(i)));
        protection = apply_delta(delta, protection, o.x, o.y);
      end
    end

    % predation
    o_inds = sub2ind( ...
        world_size, ...
        arrayfun(@(o)o.x, organisms), ...
        arrayfun(@(o)o.y, organisms) ...
    )
    predation_loss = predation_factor .^ protection(o_inds)
    absorbed = absorbed .* (1 - predation_loss)
    
    % create next generation
    %NEXT

  end

  function [absorbed_by_o] = share_sunlight(o, t)
    [d_inds t_inds] = in_bounds_indices(absorption_delta, t, o.x, o.y);
    absorbed_by_o = sum(absorption_delta(d_inds) ./ t(t_inds));
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
  
  function make_initial_population
    olocs = zeros(world_size);
    organisms = [];

    for i = 1:num_initial_organisms
      possible_locs = in_bounds(random('unid', 10, 30, 2), olocs);
      for attempt = 1:size(possible_locs,1)
        loc = possible_locs(attempt,:);
        x = loc(1); y = loc(2);
        if ~olocs(x, y)
          olocs(x, y) = 1;
          break
        end
      end
      o.g = 0.2;
      o.x = x;
      o.y = y;
      organisms = [organisms o];
    end
  end

end
