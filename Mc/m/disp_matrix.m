function disp_matrix(m)
  for row = 1:size(m,2)
    for col = 1:size(m,1)
      fprintf(' %4.2f', m(col,row));
    end
    fprintf('\n');
  end
end

