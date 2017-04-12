%% Colorimetry plotting

lightIntensity = [4170.714333	4015.111333	3872.396667	3724.492	3603.142667];
concentrations = [1 10 20 30 40];
std = [21.06038559	4.821845532	44.51070838	16.6133124	23.64332283];


P = polyfit(concentrations, lightIntensity, 1);
x = linspace(0,40,100);
fitline = polyval(P,x);



figure(1)
errorbar(concentrations, lightIntensity, std, 'k.')
hold on
plot(x,fitline)
title('Intensity of green LED light not absorbed by Rhodamine WT dye')
xlabel('Rhodamine WT concentration (ppm)')
ylabel('Green light intensity (min 0, max 65535)')



