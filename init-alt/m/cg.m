% "collective gain" simulation

function cg
  world_size = [20 20];
  a_rad = 4;   % absorption radius
  protection_factor = 1;
  predation_factor = 0.9;
  acorn_distance_factor = 0.2;
  g_sigma = 0.01;
  max_absorption = 40;
  num_initial_organisms = 20;
  num_generations = 50;

  absorption_delta = make_delta(a_rad, @(x,y)a_rad-dist([x y],[0 0]'));
                     %* diag([1 1 max_absorption]);
  protection_delta = multiply_delta(absorption_delta, protection_factor);

  % sum(absorption_delta)

  make_initial_population(@()0)

  for gen = 1:num_generations

    if gen ~= 1
      fprintf('------------------------------------------------------------\n');
    end

    % all organisms try to absorb as much sunlight as they can
    t = zeros(world_size);
    for i = 1:length(organisms)
      o = organisms(i);
      t = apply_delta(absorption_delta, t, o.x, o.y);
    end

    % actual absorption: share sunlight in contested cells
    t = max(t, ones(size(t)));
    absorbed = arrayfun(@(o)share_sunlight(o,t), organisms);
    fprintf('absorbed before predation\n');
    disp_matrix(organism_grid(@(i)absorbed(i), organisms, zeros(world_size)));

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
    fprintf('protection\n')
    disp_matrix(protection)

    % predation
    o_inds = sub2ind( ...
        world_size, ...
        arrayfun(@(o)o.x, organisms), ...
        arrayfun(@(o)o.y, organisms) ...
    );
    predation_loss = predation_factor .^ protection(o_inds);
    absorbed = absorbed .* (1 - predation_loss);
    fprintf('\nabsorbed after predation\n')
    disp_matrix(organism_grid(@(i)absorbed(i), organisms, zeros(world_size)));
    
    % create next generation
    parents = organisms;
    organisms = [];
    olocs = zeros(world_size);
    num_children = poissrnd(absorbed,size(absorbed));
    actual_children = zeros(size(absorbed));
    for p = 1:length(parents)
      for c = 1:num_children(p)
        [x, y, olocs] = find_unoccupied( ...
            acorn( ...
                parents(p).x, ...
                parents(p).y, ...
                absorbed(p) * acorn_distance_factor ...
            ), ...
            olocs ...
        );
        if x > 0
          o.x = x;
          o.y = y;
          o.g = min(max(0, normrnd(parents(p).g, g_sigma)), 1);
          organisms = [organisms o];
          actual_children(p) = actual_children(p) + 1;
        end
      end
    end
    num_children
    actual_children
    num_children - actual_children
    fprintf('%d - %d = %d children lost due to lack of space\n', ...
      sum(num_children), ...
      sum(actual_children), ...
      sum(num_children - actual_children) ...
    );

    % see the results
    disp_organisms(organisms, world_size);
    fprintf('gen = %d\n', gen)
    fprintf('# organisms = %d\n', length(organisms));
    g = arrayfun(@(o)o.g, organisms);
    fprintf('g  median=%0.2f  mean=%0.2f  min=%0.2f  max=%0.2f\n', ...
        median(100.*g), ...
        mean(100.*g), ...
        min(100.*g), ...
        max(100.*g) ...
    );
    num_givers = length(find(givers));
    fprintf('# givers=%d (%0.2f%%)\n', ...
        num_givers, 100 * num_givers / length(organisms))
    edges = linspace(0,1,101);
    histo = [edges' histc(g,edges)']

    fprintf('data %d %d %3.2f %d %d %d\n', ...
      gen, length(organisms), mean(100.*g), num_givers, sum(num_children), sum(num_children - actual_children) ...
    );

  end

  function [absorbed_by_o] = share_sunlight(o, t)
    [d_inds t_inds] = in_bounds_indices(absorption_delta, t, o.x, o.y);
    absorbed_by_o = sum(absorption_delta(d_inds) ./ t(t_inds));
  end

  function make_initial_population(gfunc)
    olocs = zeros(world_size);
    organisms = [];

    for i = 1:num_initial_organisms
      [x, y, olocs] = find_unoccupied(unidrnd(10,10,2), olocs);
      if x > 0
        o.x = x;
        o.y = y;
        o.g = gfunc();
        organisms = [organisms o];
      end
    end
  end

  function disp_organisms(organisms, world_size)
    fprintf('\norganisms\n')
    grid = -1*ones(world_size);
    for i = 1:length(organisms)
      o = organisms(i);
      grid(o.x, o.y) = o.g;
    end
    disp_g_matrix(grid);
    fprintf('\n')
  end

end
