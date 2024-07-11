function disp_g_matrix(m)
  m = 100.*m;
  for row = 1:size(m,2)
    for col = 1:size(m,1)
      g = m(row,col);
      if g < 0
        fprintf('    -');
      else
        fprintf('%5.0f', g);
      end
    end
    fprintf('\n');
  end
end

