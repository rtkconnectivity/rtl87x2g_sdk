
from multiprocessing import Process
import argparse
import subprocess
import os
import re
import csv



paa_der = ""
pai_der = ""
cd_der = ""
factory_gen_script = os.path.abspath("factory_data.py")
vid = 0
pid = 0
chip_cert = ""


def find_PAI():
    pat = re.compile(".*PAI-([\dA-F]{4}).*\.der")
    for file_name in os.listdir():
        match_obj: re.Match = pat.match(file_name)
        if match_obj:
            if match_obj.group(1).lower() == ("%04X" % vid).lower():
                return os.path.abspath(file_name)
    return None

def find_PAA():
    pat = re.compile(".*-PAA-.*\.der")
    for file_name in os.listdir():
        match_obj: re.Match = pat.match(file_name)
        if match_obj:
            return os.path.abspath(file_name)
            
    return None

def find_CD():
    pat = re.compile(".*-CD-([\dA-F]{4})-([\dA-F]{4}).*\.der")
    for file_name in os.listdir():
        match_obj: re.Match = pat.match(file_name)
        if match_obj:
            if match_obj.group(1).lower() == ("%04X" % vid).lower() and match_obj.group(2).lower() == ("%04X" % pid).lower():
                return os.path.abspath(file_name)


def get_vid_pid():
    for file_name in os.listdir():
        pat = re.compile("DAC-([\dA-F]{4})-([\dA-F]{4})-([\dA-F]{5})")
        match_obj: re.Match = pat.match(file_name)
        if match_obj:
            vid = int(match_obj.group(1), 16)
            pid = int(match_obj.group(2), 16)
            return vid, pid
    
    return None


def gen_bin(dir_name, batch_no, unit_no):
    global vid, pid, chip_cert, paa_der, pai_der
    
    os.chdir(dir_name)
    unit_dir = os.path.abspath(".")
    vid_str = "%04X" % vid
    pid_str = "%04X" % pid
    sn_str = "%016X" % sn
    batch_no_str = "%05X" % batch_no
    unit_no_str = "%08X" % unit_no
    
    dac_priv_key = os.path.join(unit_dir,  "DAC-" + vid_str + "-" + pid_str + "-PrivateKey-" + batch_no_str + "-" + unit_no_str + ".der.enc")
    dac_der: str = os.path.join(unit_dir,  "DAC-" + vid_str + "-" + pid_str + "-Cert-" + batch_no_str + "-" + unit_no_str + ".der")
    
    script_dir, script = os.path.split(factory_gen_script)
    
    os.chdir(script_dir)
    
    cmd = ["python3", script, 
            "-p", "20202021", 
            "-d", "3840",
            "--spake2p_path=./spake2p",
            "--vendor-id", "0x" + vid_str,
            "--vendor-name", "Realtech",
            "--product-id", "0x" + pid_str,
            "--product-name", "bee4",
            "--serial-num", sn_str,
            "--hw-ver", "1",
            "--hw-ver-str", "1.0",
            "--rd-id-uid", "00112233445566778899aabbccddeeff",
            "--dac_cert", dac_der, 
            "--dac_key", dac_priv_key,
            "--pai_cert", pai_der,
            "--paa_cert", paa_der,
            "--cd", cd_der,
            ]
    
    if args.chip_cert:          
            cmd += ["--chip_cert", chip_cert]
    
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    
    # 获取标准输出和标准错误
    stdout, stderr = process.communicate()

    # 打印标准输出和标准错误
    print("Standard Output:\n", stdout.decode())
    print("Standard Error:\n", stderr.decode())
    if process.returncode:
        print("certs check failed!")
        raise
    
    output_dir_name = os.path.join(rtk_pki_dir, "device_" + vid_str + "_" + pid_str, sn_str)
    if not os.path.exists(output_dir_name):
        os.makedirs(output_dir_name)
    
    os.rename("factory_data.bin", os.path.join(output_dir_name, "factory_data_" + sn_str + ".bin"))
    with open("qrcode.txt", "r") as f:
        MT_str = f.read()
        re_match_obj = re.search("(MT:.*)", MT_str)
        pure_MT_str = re_match_obj.group(1)
        pairing_code_path = os.path.join(output_dir_name, "pairing_code.txt")
        with open(pairing_code_path, "w") as fp:
            fp.write(pure_MT_str)
    
    os.rename("qrcode.png", os.path.join(output_dir_name, "qrcode.png"))
    
    os.chdir(unit_dir)
    
    os.chdir("..")


def write_csv(batch_no, unit_no):
    global sn, vid, pid, rtk_pki_dir
    vid_str = "%04X" % vid
    pid_str = "%04X" % pid
    sn_str = "%016X" % sn
    batch_no_str = "%05X" % batch_no
    unit_no_str = "%08X" % unit_no
    csv_path = os.path.join(rtk_pki_dir, "sn_device_map.csv")
    csv_exists = os.path.exists(csv_path)
    with open(csv_path, mode='a+', encoding='utf-8') as f:
        writer = csv.writer(f)
        if not csv_exists:
            writer.writerow(["sn", "vid", "pid", "batch_no", "unit_no"])
        writer.writerow([sn_str, vid_str, pid_str, batch_no_str, unit_no_str])


def traverse_batch(dir_name, batch_no):
    global sn, vid, pid
    os.chdir(dir_name)
    pat = re.compile("DAC-([\dA-F]{4})-([\dA-F]{4})-([\dA-F]{5})-([\d]{8})")
    for dir_name in os.listdir():
        match_obj: re.Match = pat.match(dir_name)
        if match_obj:
            unit_no = int(match_obj.group(4), 16)
            gen_bin(dir_name, batch_no, unit_no)
            write_csv(batch_no, unit_no)
            sn += 1
            
    os.chdir("..")


def any_base_int(s):
    return int(s, 0)

if __name__ == '__main__':
    
    parser = argparse.ArgumentParser(description='Realtech Factory NVS Data invoker')
    
    parser.add_argument('--rtk_dir', type=str, required=True,
                        help='The path to the rtk certs')
   
    parser.add_argument('--sn', type=any_base_int, required=True,
                        help='sequence number of device')
    
    parser.add_argument('--chip_cert', type=str, required=False,
                        help='The path to the rtk certs')
    
    
    args = parser.parse_args()
    
    sn: int = args.sn
    rtk_pki_dir = args.rtk_dir
    rtk_pki_dir = os.path.abspath(rtk_pki_dir)
    chip_cert = args.chip_cert
    
    
    os.chdir(rtk_pki_dir)
    vid, pid = get_vid_pid()
    pai_der = find_PAI()
    paa_der = find_PAA()
    cd_der = find_CD()
    
    pat = re.compile("DAC-([\dA-F]{4})-([\dA-F]{4})-([\dA-F]{5})")
    for dir_name in os.listdir():
        match_obj: re.Match = pat.match(dir_name)
        if match_obj:
            batch_no = int(match_obj.group(3), 16)
            traverse_batch(dir_name, batch_no)
        
    
    # for dir_name in os.listdir():
    #     pat = re.compile("DAC-([\dA-F]{4})-([\dA-F]{4})-([\dA-F]{5})-([\d]{8})")
    #     match_obj: re.Match = pat.match(dir_name)
    #     if match_obj:
    #         sn_str, vid_str, pid_str, device_seq_str = generate_bin(match_obj)
    #         csv_exists = os.path.exists("sn_device_seq_map.csv")
    #         with open("sn_device_seq_map.csv", mode='a+', encoding='utf-8') as f:
    #             writer = csv.writer(f)
    #             if not csv_exists:
    #                 writer.writerow(["sn", "device_seq", "vid", "pid"])
    #             writer.writerow([sn_str, device_seq_str, vid_str, pid_str])













