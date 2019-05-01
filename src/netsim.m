function stats = netsim(varargin)

nSamples = 1;
% Simulation parameters defaults values
defParams = {...
  'algorithm',      'A', ...
  'sSinrThr',       10, ...  % umbral de SINR de los enlaces secundarios (dB)
  'pInterfThr',     -115, ...  % primary interference threshold (dBm)
  'sOutageThr',     1e-3, ...  % umbral de corte de los enlaces secundarios
  'sPower',         NaN, ...%[20 20], ...   % potencia de los enlaces secundarios
  'sPowerMin',      0,  ...    % potencia minima de los enlaces secundarios
  'sPowerMax',      25, ...% [26.9219   26.9630],  ...   % potencia maxima de los enlaces primarios (dBm)
  'eqPower',        false, ...
  'pPower',         43, ...    % primary trasmit power (dBm)
  'rho',            1, ... %'topology',       [100 10 50], ...
  'd1',             300, ...
  'd2',             30, ...
  'd3',             125, ...
  'd4',             0 ...
  };

for k = 1:2:numel(defParams)
  val = setparam(defParams{k}, varargin, defParams);
  params.(defParams{k}) = val;
end

params.topology = [params.d1 params.d2 params.d3 params.d4];

% Convert dB values to linear values
params.sPower = 10.^(params.sPower/10);
params.sSinrThr = 10.^(params.sSinrThr/10);
params.pPower = 10.^(params.pPower/10);
params.pInterfThr = 10.^(params.pInterfThr/10);
params.sPowerMax = 10.^(params.sPowerMax/10);

% Simulation scenary definition
net = struct(... 
  'topology', params.topology, ...
  'show',     false ...
  );

channels = setnet(net);

% Set auxiliary variables
Os = params.sOutageThr;
Rs = channels.sr/(params.sSinrThr*params.pPower*channels.pr);
Rr = channels.rd/(params.sSinrThr*params.pPower*channels.pd);
R = [Rs, Rr];
Ss = params.pInterfThr/channels.sp;
Sr = params.pInterfThr/channels.rp;
S = [Ss, Sr];

switch params.algorithm
  case {'A'}
    % Analytical Results
    %   Optimal power allocation
    if isnan(params.sPower)
      % Minimum power that meet the outage probability
      power = (R*(1 - Os) + sqrt( R(1)*R(2)*(1 - Os) ))/( R(1)*R(2)*Os );
      % Check if this power allocation violates the primary interference
      % requeriment. If so, then allocate the power to minimize the outage 
      % probability but guarentening the primery interference tehershold
      if (0.5*(channels.sp*power(1) + channels.rp*power(2)) > params.pInterfThr)
        power = 2*S./( 1 + sqrt( (1 + 2*R.*S)./(1 + 2*R(end:-1:1).*S(end:-1:1)) ) );
      end
    else
      power = params.sPower;
    end
    power = min(power, params.sPowerMax);
    %   Outage probability
    sOutageTheo = 1 - prod(R.*power./(1 + R.*power));
    %   Average interfrence at primary receiver
    pInterfTheo = 0.5*( channels.sp*power(1) + channels.rp*power(2) );
    %   Power consumptiom
    sTotalPowerTheo = mean(power);
    
    % Simulation Results
    fields = fieldnames(channels);
    vrepm = ones(nSamples, 1);
    for n = 1:numel(fields)
       h.(fields{n}) = exprnd( channels.(fields{n})(vrepm, :) );
    end
    %   Outage probability of secondary users
    srSinr = power(1)*h.sr./(params.pPower*h.pr);
    rdSinr = power(2)*h.rd./(params.pPower*h.pd);
    sOutageSim = nnz( min(srSinr, rdSinr) < params.sSinrThr)/nSamples;
    %   Power consumptiom
    sTotalPowerSim = mean(power);
    %   Average interfrence at primary receiver
    pInterfSim = 0.5*( mean(power(1)*h.sp) + mean(power(2)*h.rp) );
    %   Primary outage probability
    pOutageSim = nnz( [power(1)*h.sp, power(2)*h.rp] > params.pInterfThr)/(2*nSamples);  
  case {'B'}
    if (params.eqPower)          
      % Compute the transmission power based on channels statistical information
      if isnan(params.sPower)
        % Minimum power that meet the outage probability
        power = (R*(1 - Os) + sqrt( R(1)*R(2)*(1 - Os) ))/( R(1)*R(2)*Os );
        % Check if this power allocation violates the primary interference
        % requeriment. If so, then allocate the power to minimize the outage 
        % probability but guarentening the primery interference tehershold
        if (0.5*(channels.sp*power(1) + channels.rp*power(2)) > params.pInterfThr)
          power = 2*S./( 1 + sqrt( (1 + 2*R.*S)./(1 + 2*R(end:-1:1).*S(end:-1:1)) ) );
        end
      else
        power = params.sPower;
      end
      % Determine the peak power such that the average power consumption is equal the transmission 
      % power based on channels statistical information
      powerMax(1) = fzero(...
        @(pMax)(pMax*(1 - exp(-S(1)/pMax)) + S(1)*expint(S(1)/pMax) - power(1)),...
        [0.1*power(1), 10000*power(1)]);
      powerMax(2) = fzero(...
        @(pMax)(pMax*(1 - exp(-S(2)/pMax)) + S(2)*expint(S(2)/pMax) - power(2)),...
        [0.1*power(2), 10000*power(2)]);
      %powerMax = power;
    else
      powerMax = params.sPowerMax*ones(1, 2);
    end
    % Auxiliary variables
    fields = fieldnames(channels);
    vrepm = ones(nSamples, 1);
    % Set channels instanteneus values
    for n = 1:numel(fields)
       h.(fields{n}) = exprnd( channels.(fields{n})(vrepm, :) );
    end

    % Set the transmission power
    hsp = (params.rho*sqrt(h.sp) + sqrt(1 - params.rho.^2)*sqrt(channels.sp)*randn( size(h.sp) )).^2;
    hrp = (params.rho*sqrt(h.rp) + sqrt(1 - params.rho.^2)*sqrt(channels.rp)*randn( size(h.rp) )).^2;
    sPower = min(params.pInterfThr./hsp, powerMax(1));
    rPower = min(params.pInterfThr./hrp, powerMax(2));
    
    % Analytical results
    %   Outage Probability
    O = (1 + powerMax.*R.*exp(-S./powerMax))./(1 + powerMax.*R)...
      - ( R.*S.*exp(R.*S) ).*expint(R.*S + S./powerMax);
    sOutageTheo = 1 - prod(1-O);
    %   Power consumption
    sTotalPowerTheo = mean(powerMax.*(1 - exp(-S./powerMax)) + S.*expint(S./powerMax));
    %   Average interfrence at primary receiver
    pInterfTheo = NaN;
    
    % Simulation results
    %   Average power consumption
    sTotalPowerSim = 0.5*(mean(sPower) + mean(rPower));
    %   Outage probability
    srSinr = sPower.*h.sr./(params.pPower*h.pr);
    rdSinr = rPower.*h.rd./(params.pPower*h.pd);
    sOutageSim = nnz( min(srSinr, rdSinr) < params.sSinrThr)/nSamples;
    %   Average interference at primary receiver
    pInterfSim = 0.5*(mean(sPower.*h.sp) + mean(rPower.*h.rp));
    %   Primary outage probability
    pOutageSim = nnz( [sPower.*h.sp, rPower.*h.rp] > params.pInterfThr)/(2*nSamples);  
end

stats = struct(...
  'sTotalPowerSim', 10*log10(sTotalPowerSim), ...
  'sTotalPowerTheo', 10*log10(sTotalPowerTheo), ...
  'sOutageSim', sOutageSim, ...
  'sOutageTheo', sOutageTheo, ...
  'pInterfSim', 10*log10(pInterfSim), ...
  'pInterfTheo', 10*log10(pInterfTheo), ...  
  'pOutageSim', pOutageSim ...
  );