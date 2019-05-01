function paramValue = setparam(paramName, params, defParams)

% First the specified parameters
for k=1:2:numel(params)
  if strcmpi(paramName, params{k})
    paramValue = params{k + 1};
    return;
  end        
end

% Next search in default values
for k=1:2:numel(defParams)
  if strcmpi(paramName, defParams{k})
    paramValue = defParams{k + 1};
    return;
  end        
end

error('Parameter %s not found', upper(paramName));


