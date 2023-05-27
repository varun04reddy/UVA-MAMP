clearvars

path = 'D:\varun\UniversityofVirginia\mamp';
all_animals = {
    'BAA01';'BAA02';'BAA03';'BAA04';
    'BAA05';'BAA06';'BAA07';'BAA08';
    'BAA09';'BAA10';'BAA11';'BAA12';
    'BAA13';'BAA14';'BAA15';'BAA16';
    'BAA17';'BAA18';'BAA19';'BAA20';
    'BAA21';'BAA22';'BAA23';'BAA24';
    'BAA25';'BAA26';'BAA27';'BAA28';
    'BAA29';'BAA30';'BAA31';'BAA32'};  
G = [];
for animal = 1:length(all_animals)
    trials = dir(fullfile(path,all_animals{animal},'Baseline','*_Walk*_ik.mot'));
    for t = 1:length(trials)
        trial_id = replace(trials(t).name,'_ik.mot','.mat');
        mat_file = load(fullfile(trials(t).folder,trial_id));
        kinematics = readtable(fullfile(trials(t).folder,trials(t).name),'FileType','text');
        cam_freq = mat_file.Info.ViconCameraFrameRate;
        RFS1 = mat_file.Events.RightFootStrike(1)/cam_freq;
        RFS2 = mat_file.Events.RightFootStrike(2)/cam_freq;
        LFS1 = mat_file.Events.LeftFootStrike(1)/cam_freq;
        LFS2 = mat_file.Events.LeftFootStrike(2)/cam_freq;
        kinematics_right = kinematics(kinematics.time >= RFS1 & kinematics.time <= RFS2,:);
        kinematics_left = kinematics(kinematics.time >= LFS1 & kinematics.time <= LFS2,:);
        cutoff = 15;
        Wn = cutoff/(0.5*cam_freq);
        [b,a]  = butter(4,Wn,'low');
        kinematics_filt_right = filtfilt(b,a,table2array(kinematics_right(:,2:end)));
        kinematics_filt_left = filtfilt(b,a,table2array(kinematics_left(:,2:end)));  
        stance_right = kinematics_right.time(kinematics_right.time >= RFS1 & kinematics_right.time <= RFS2);
        time = stance_right - stance_right(1);
        gphase = 0:time(end)/100:time(end);
        kinematics_splined_right = spline(time,kinematics_filt_right',gphase)';
        kinematics_splined_right = array2table(kinematics_splined_right,'VariableNames',kinematics.Properties.VariableNames(2:end));
        stance_left = kinematics.time(kinematics.time >= LFS1 & kinematics.time <= LFS2);
        time = stance_left - stance_left(1);
        gphase = 0:time(end)/100:time(end);
        kinematics_splined_left = spline(time,kinematics_filt_left',gphase)';
        kinematics_splined_left = array2table(kinematics_splined_left,'VariableNames',kinematics.Properties.VariableNames(2:end));
        coord_list_r = {'sacrum_pitch','sacrum_roll','sacrum_yaw','hip_r_flx','hip_r_add','hip_r_int','knee_r_flx','ankle_r_flx'};
        coord_list_l = {'sacrum_pitch','sacrum_roll','sacrum_yaw','hip_l_flx','hip_l_add','hip_l_int','knee_l_flx','ankle_l_flx'};
        kinematics_subsample_right = kinematics_splined_right(1:2:end,contains(kinematics_splined_right.Properties.VariableNames,coord_list_r));
        kinematics_subsample_left = kinematics_splined_left(1:2:end,contains(kinematics_splined_left.Properties.VariableNames,coord_list_l));
        trial_matrix = [];
        for col = 1:width(kinematics_subsample_right)
            if contains(kinematics_subsample_left.Properties.VariableNames(col),{'roll','yaw','ext','abd'})
                trial_matrix = [trial_matrix;[-kinematics_subsample_left{:,col}, kinematics_subsample_right{:,col}]];
            elseif contains(kinematics_subsample_left.Properties.VariableNames(col),{'knee'})
                trial_matrix = [trial_matrix;[-kinematics_subsample_left{:,col}, -kinematics_subsample_right{:,col}]];
            else
                trial_matrix = [trial_matrix;[kinematics_subsample_left{:,col}, kinematics_subsample_right{:,col}]];
            end
        end
        G = [G,trial_matrix];
    end
    disp(['Done with ' all_animals{animal} '.']);
end

save('GaitMatrix.mat','G')
