# Origami: A High-Performance Mergesort Framework

[![GPLv3 license](https://img.shields.io/badge/License-GPLv3-blue.svg?style=plastic&logo=appveyor)](LICENSE)

This repository contains the implementation of the components and algorithms of the Origami framework described in the [**Origami paper**](https://doi.org/10.14778/3489496.3489507). The framework was developed by the [**IRL @ Texas A&M University**](http://irl.cs.tamu.edu/projects/streams/), published in the [**PVLDB Volume 15**](https://vldb.org/pvldb/vol15-volume-info/), and will be presented in the [**VLDB 2022**](https://vldb.org/2022/).

## Abstract

Mergesort is a popular algorithm for sorting real-world workloads as it is immune to data skewness, suitable for parallelization using vectorized intrinsics, and relatively simple to multithread. In this paper, we introduce Origami, an in-memory mergesort framework that is optimized for scalar, as well as all current SIMD (single-instruction multiple-data) CPU architectures. For each vector-extension set (e.g., SSE, AVX2, AVX-512), we present an inregister sorter for small sequences that is up to 8× faster than prior methods and a branchless streaming merger that achieves up to a 1.5x speed-up over the naive merge. In addition, we introduce a cache-residing quad-merge tree to avoid bottlenecking on memory bandwidth and a parallel partitioning scheme to maximize threadlevel concurrency. We develop an end-to-end sort with these components and produce a highly utilized mergesort pipeline by reducing the synchronization overhead between threads. Single-threaded Origami performs up to 2x faster than the closest competitor and achieves a nearly perfect speed-up in multi-core environments.

## Performance

A quick preview of Origami sort performance is given in the tables below. For a deeper analysis and discussion, please refer to the paper. Benchmarks run all code compiled with Visual Studio 2019 in Windows Server 2016 on an 8-core Intel Skylake-X (i7-7820X) CPU with a fixed 4.7 GHz clock on all cores, 1 MB L2 cache, and 32 GB of DDR4-3200 quad-channel RAM. When AVX-512 was used, BIOS defaulted to a 400-MHz lower clock (i.e., 4.3 GHz), which is known as the AVX offset implemented by many motherboard manufacturers to keep core temperature under control. We enable the maximum level of optimization and use appropriate flags (e.g., /arch:AVX512) to ensure the compiler uses all available SIMD registers.

The following table shows the performance comparison of Origami with prior works in corresponding SIMD architectures. It compares the sort speed of different chunk sizes in a 1 GB array of 32-bit random keys.

<table>
<thead>
  <tr>
    <th colspan="10">Single Thread Chunked Sort Speed (M keys/s, 32-bit keys)</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td align="center" rowspan="2"><b><sub>Chunk size</sub></b></td>
    <td align="center" colspan="2"><b><sub>SSE</sub></b></td>
    <td align="center" colspan="3"><b><sub>AVX2</sub></b></td>
    <td align="center" colspan="4"><b><sub>AVX-512</sub></b></td>
  </tr>
  <tr>
    <td align="center"><b><sub><a href="https://dl.acm.org/doi/pdf/10.14778/2809974.2809988">AA-sort</a></sub></b></td>
    <td align="center"><b><sub>Origami</sub></b></td>
    <td align="center"><b><sub><a href="https://github.com/vtsynergy/aspas_sort">ASPaS-sort</a></sub></b></td>
    <td align="center"><b><sub><a href="https://github.com/sid1607/avx2-merge-sort">Peloton-sort</a></sub></b></td>
    <td align="center"><b><sub>Origami</sub></b></td>
    <td align="center"><b><sub><a href="https://github.com/jalexw1013/avx-sort">Watkins-sort</a></sub></b></td>
    <td align="center"><b><sub><a href="https://dl.acm.org/doi/pdf/10.1145/2535753.2535762">Xiaochen-sort</a></sub></b></td>
    <td align="center"><b><sub><a href="https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=8855628">Yin-sort</a></sub></b></td>
    <td align="center"><b><sub>Origami</sub></b></td>
  </tr>
  <tr>
    <td align="center"><sub>128 K</sub></td>
    <td align="center"><sub>63</sub></td>
    <td align="center"><sub>176</sub></td>
    <td align="center"><sub>53</sub></td>
    <td align="center"><sub>139</sub></td>
    <td align="center"><sub>228</sub></td>
    <td align="center"><sub>40</sub></td>
    <td align="center"><sub>198</sub></td>
    <td align="center"><sub>140</sub></td>
    <td align="center"><sub>295</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>256 K</sub></td>
    <td align="center"><sub>61</sub></td>
    <td align="center"><sub>147</sub></td>
    <td align="center"><sub>47</sub></td>
    <td align="center"><sub>128</sub></td>
    <td align="center"><sub>210</sub></td>
    <td align="center"><sub>33</sub></td>
    <td align="center"><sub>184</sub></td>
    <td align="center"><sub>130</sub></td>
    <td align="center"><sub>269</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>512 K</sub></td>
    <td align="center"><sub>59</sub></td>
    <td align="center"><sub>138</sub></td>
    <td align="center"><sub>44</sub></td>
    <td align="center"><sub>120</sub></td>
    <td align="center"><sub>195</sub></td>
    <td align="center"><sub>30</sub></td>
    <td align="center"><sub>172</sub></td>
    <td align="center"><sub>113</sub></td>
    <td align="center"><sub>249</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>1 M</sub></td>
    <td align="center"><sub>57</sub></td>
    <td align="center"><sub>131</sub></td>
    <td align="center"><sub>41</sub></td>
    <td align="center"><sub>109</sub></td>
    <td align="center"><sub>183</sub></td>
    <td align="center"><sub>28</sub></td>
    <td align="center"><sub>160</sub></td>
    <td align="center"><sub>102</sub></td>
    <td align="center"><sub>232</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>2 M</sub></td>
    <td align="center"><sub>55</sub></td>
    <td align="center"><sub>124</sub></td>
    <td align="center"><sub>39</sub></td>
    <td align="center"><sub>92</sub></td>
    <td align="center"><sub>174</sub></td>
    <td align="center"><sub>25</sub></td>
    <td align="center"><sub>150</sub></td>
    <td align="center"><sub>95</sub></td>
    <td align="center"><sub>216</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>4 M</sub></td>
    <td align="center"><sub>54</sub></td>
    <td align="center"><sub>118</sub></td>
    <td align="center"><sub>37</sub></td>
    <td align="center"><sub>81</sub></td>
    <td align="center"><sub>168</sub></td>
    <td align="center"><sub>23</sub></td>
    <td align="center"><sub>140</sub></td>
    <td align="center"><sub>88</sub></td>
    <td align="center"><sub>203</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>8 M</sub></td>
    <td align="center"><sub>52</sub></td>
    <td align="center"><sub>112</sub></td>
    <td align="center"><sub>35</sub></td>
    <td align="center"><sub>77</sub></td>
    <td align="center"><sub>162</sub></td>
    <td align="center"><sub>21</sub></td>
    <td align="center"><sub>131</sub></td>
    <td align="center"><sub>83</sub></td>
    <td align="center"><sub>191</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>16 M</sub></td>
    <td align="center"><sub>50</sub></td>
    <td align="center"><sub>107</sub></td>
    <td align="center"><sub>33</sub></td>
    <td align="center"><sub>73</sub></td>
    <td align="center"><sub>153</sub></td>
    <td align="center"><sub>20</sub></td>
    <td align="center"><sub>122</sub></td>
    <td align="center"><sub>78</sub></td>
    <td align="center"><sub>181</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>32 M</sub></td>
    <td align="center"><sub>48</sub></td>
    <td align="center"><sub>102</sub></td>
    <td align="center"><sub>32</sub></td>
    <td align="center"><sub>70</sub></td>
    <td align="center"><sub>145</sub></td>
    <td align="center"><sub>19</sub></td>
    <td align="center"><sub>115</sub></td>
    <td align="center"><sub>72</sub></td>
    <td align="center"><sub>172</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>64 M</sub></td>
    <td align="center"><sub>47</sub></td>
    <td align="center"><sub>98</sub></td>
    <td align="center"><sub>30</sub></td>
    <td align="center"><sub>67</sub></td>
    <td align="center"><sub>138</sub></td>
    <td align="center"><sub>18</sub></td>
    <td align="center"><sub>109</sub></td>
    <td align="center"><sub>69</sub></td>
    <td align="center"><sub>163</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>128 M</sub></td>
    <td align="center"><sub>45</sub></td>
    <td align="center"><sub>95</sub></td>
    <td align="center"><sub>29</sub></td>
    <td align="center"><sub>65</sub></td>
    <td align="center"><sub>132</sub></td>
    <td align="center"><sub>17</sub></td>
    <td align="center"><sub>103</sub></td>
    <td align="center"><sub>66</sub></td>
    <td align="center"><sub>156</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>256 M</sub></td>
    <td align="center"><sub>44</sub></td>
    <td align="center"><sub>91</sub></td>
    <td align="center"><sub>28</sub></td>
    <td align="center"><sub>63</sub></td>
    <td align="center"><sub>126</sub></td>
    <td align="center"><sub>17</sub></td>
    <td align="center"><sub>97</sub></td>
    <td align="center"><sub>64</sub></td>
    <td align="center"><sub>149</sub></td>
  </tr>
</tbody>
</table>


Origami is distribution insensitive i.e. it retains almost constant speed on all inputs. The next table shows this characteristic on 1 GB data for the following distributions:

D1: Uniformly random, generated by Mersenne Twister\
D2: All same keys\
D3: Sorted\
D4: Reverse sorted\
D5: Almost sorted, where every 7th key is set to KEY_MAX\
D6: Pareto keys, generated as min(ceil(beta(1/(1-u) - 1)), 10000), where beta = 7 and u ~ uniform[0, 1]\
D7: Bursts of same keys, where the length of each subsequence is drawn from D6 and key from D1\
D8: Random shuffle, generated by randomly permuting D7\
D9: Fibonacci, wrapped around when overflows number of items to sort


<table>
<thead>
  <tr>
    <th colspan="11">Single Thread Sort Speed for Different Distributions (M keys/s)</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td align="center" colspan="1"></td>
    <td align="center" colspan="1"><b><sub>Key size</sub></b></td>
    <td align="center" colspan="1"><b><sub>D1</sub></b></td>
    <td align="center" colspan="1"><b><sub>D2</sub></b></td>
    <td align="center" colspan="1"><b><sub>D3</sub></b></td>
    <td align="center" colspan="1"><b><sub>D4</sub></b></td>
    <td align="center" colspan="1"><b><sub>D5</sub></b></td>
    <td align="center" colspan="1"><b><sub>D6</sub></b></td>
    <td align="center" colspan="1"><b><sub>D7</sub></b></td>
    <td align="center" colspan="1"><b><sub>D8</sub></b></td>
    <td align="center" colspan="1"><b><sub>D9</sub></b></td>    
  </tr>
  <tr>
    <td align="center" rowspan="3"> <sub>Scalar</sub> </td>
    <td align="center"> <sub>32</sub> </td>
    <td align="center"> <sub>47</sub> </td>
    <td align="center"> <sub>52</sub> </td>
    <td align="center"> <sub>52</sub> </td>
    <td align="center"> <sub>52</sub> </td>
    <td align="center"> <sub>47</sub> </td>
    <td align="center"> <sub>47</sub> </td>
    <td align="center"> <sub>49</sub> </td>
    <td align="center"> <sub>47</sub> </td>
    <td align="center"> <sub>47</sub> </td>
  </tr>
  <tr>
    <td align="center"> <sub>64</sub> </td>
    <td align="center"> <sub>43</sub> </td>
    <td align="center"> <sub>47</sub> </td>
    <td align="center"> <sub>47</sub> </td>
    <td align="center"> <sub>47</sub> </td>
    <td align="center"> <sub>43</sub> </td>
    <td align="center"> <sub>44</sub> </td>
    <td align="center"> <sub>45</sub> </td>
    <td align="center"> <sub>43</sub> </td>
    <td align="center"> <sub>44</sub> </td>
  </tr>
  <tr>
    <td align="center"> <sub>64+64</sub> </td>
    <td align="center"> <sub>25</sub> </td>
    <td align="center"> <sub>27</sub> </td>
    <td align="center"> <sub>27</sub> </td>
    <td align="center"> <sub>27</sub> </td>
    <td align="center"> <sub>25</sub> </td>
    <td align="center"> <sub>25</sub> </td>
    <td align="center"> <sub>25</sub> </td>
    <td align="center"> <sub>25</sub> </td>
    <td align="center"> <sub>25</sub> </td>
  </tr>
  <tr>
    <td align="center" rowspan="3"> <sub>SSE</sub> </td>
    <td align="center"> <sub>32</sub> </td>
    <td align="center"> <sub>91</sub> </td>
    <td align="center"> <sub>90</sub> </td>
    <td align="center"> <sub>90</sub> </td>
    <td align="center"> <sub>90</sub> </td>
    <td align="center"> <sub>91</sub> </td>
    <td align="center"> <sub>91</sub> </td>
    <td align="center"> <sub>92</sub> </td>
    <td align="center"> <sub>91</sub> </td>
    <td align="center"> <sub>90</sub> </td>
  </tr>
  <tr>
    <td align="center"> <sub>64</sub> </td>
    <td align="center"> <sub>50</sub> </td>
    <td align="center"> <sub>49</sub> </td>
    <td align="center"> <sub>49</sub> </td>
    <td align="center"> <sub>50</sub> </td>
    <td align="center"> <sub>50</sub> </td>
    <td align="center"> <sub>50</sub> </td>
    <td align="center"> <sub>50</sub> </td>
    <td align="center"> <sub>50</sub> </td>
    <td align="center"> <sub>49</sub> </td>
  </tr>
  <tr>
    <td align="center"> <sub>64+64</sub> </td>
    <td align="center"> <sub>35</sub> </td>
    <td align="center"> <sub>34</sub> </td>
    <td align="center"> <sub>34</sub> </td>
    <td align="center"> <sub>34</sub> </td>
    <td align="center"> <sub>35</sub> </td>
    <td align="center"> <sub>35</sub> </td>
    <td align="center"> <sub>35</sub> </td>
    <td align="center"> <sub>35</sub> </td>
    <td align="center"> <sub>34</sub> </td>
  </tr>
  <tr>
    <td align="center" rowspan="3"> <sub>AVX2</sub> </td>
    <td align="center"> <sub>32</sub> </td>
    <td align="center"> <sub>126</sub> </td>
    <td align="center"> <sub>126</sub> </td>
    <td align="center"> <sub>125</sub> </td>
    <td align="center"> <sub>124</sub> </td>
    <td align="center"> <sub>126</sub> </td>
    <td align="center"> <sub>126</sub> </td>
    <td align="center"> <sub>127</sub> </td>
    <td align="center"> <sub>126</sub> </td>
    <td align="center"> <sub>125</sub> </td>
  </tr>
  <tr>
    <td align="center"> <sub>64</sub> </td>
    <td align="center"> <sub>48</sub> </td>
    <td align="center"> <sub>48</sub> </td>
    <td align="center"> <sub>48</sub> </td>
    <td align="center"> <sub>48</sub> </td>
    <td align="center"> <sub>48</sub> </td>
    <td align="center"> <sub>48</sub> </td>
    <td align="center"> <sub>48</sub> </td>
    <td align="center"> <sub>48</sub> </td>
    <td align="center"> <sub>47</sub> </td>
  </tr>
  <tr>
    <td align="center"> <sub>64+64</sub> </td>
    <td align="center"> <sub>34</sub> </td>
    <td align="center"> <sub>34</sub> </td>
    <td align="center"> <sub>34</sub> </td>
    <td align="center"> <sub>34</sub> </td>
    <td align="center"> <sub>34</sub> </td>
    <td align="center"> <sub>34</sub> </td>
    <td align="center"> <sub>34</sub> </td>
    <td align="center"> <sub>34</sub> </td>
    <td align="center"> <sub>34</sub> </td>
  </tr>
  <tr>
    <td align="center" rowspan="3"> <sub>AVX-512</sub> </td>
    <td align="center"> <sub>32</sub> </td>
    <td align="center"> <sub>149</sub> </td>
    <td align="center"> <sub>145</sub> </td>
    <td align="center"> <sub>145</sub> </td>
    <td align="center"> <sub>145</sub> </td>
    <td align="center"> <sub>148</sub> </td>
    <td align="center"> <sub>149</sub> </td>
    <td align="center"> <sub>149</sub> </td>
    <td align="center"> <sub>149</sub> </td>
    <td align="center"> <sub>146</sub> </td>
  </tr>
  <tr>
    <td align="center"> <sub>64</sub> </td>
    <td align="center"> <sub>65</sub> </td>
    <td align="center"> <sub>63</sub> </td>
    <td align="center"> <sub>64</sub> </td>
    <td align="center"> <sub>63</sub> </td>
    <td align="center"> <sub>64</sub> </td>
    <td align="center"> <sub>64</sub> </td>
    <td align="center"> <sub>65</sub> </td>
    <td align="center"> <sub>64</sub> </td>
    <td align="center"> <sub>63</sub> </td>
  </tr>
  <tr>
    <td align="center"> <sub>64+64</sub> </td>
    <td align="center"> <sub>27</sub> </td>
    <td align="center"> <sub>26</sub> </td>
    <td align="center"> <sub>26</sub> </td>
    <td align="center"> <sub>26</sub> </td>
    <td align="center"> <sub>26</sub> </td>
    <td align="center"> <sub>27</sub> </td>
    <td align="center"> <sub>26</sub> </td>
    <td align="center"> <sub>26</sub> </td>
    <td align="center"> <sub>26</sub> </td>
  </tr>
</tbody>
</table>


Origami achieves near perfect speedup in multi-core environments. The next table shows the scalability for 1 GB random data:

<table>
<thead>
  <tr>
    <th colspan="9">Parallel Sort Speed on Skylake-X (M/s)</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td align="center" rowspan="2"></td>
    <td align="center" rowspan="2">Key size</td>
    <td align="center" colspan="4"><b><sub>Speed (M/s)</sub></b></td>
    <td align="center" colspan="3"><b><sub>Speed-up</sub></b></td>  
  </tr>
  <tr>
    <td align="center" colspan="1"><sub>1C</sub></td>
    <td align="center" colspan="1"><sub>2C</sub></td>   
    <td align="center" colspan="1"><sub>4C</sub></td>   
    <td align="center" colspan="1"><sub>8C</sub></td>   
    <td align="center" colspan="1"><sub>2C</sub></td>   
    <td align="center" colspan="1"><sub>4C</sub></td>   
    <td align="center" colspan="1"><sub>8C</sub></td>   
  </tr>
  <tr>
    <td align="center" rowspan="3"><sub>Scalar</sub></td>
    <td align="center"><sub>32</sub></td>
    <td align="center"><sub>47</sub></td>
    <td align="center"><sub>74</sub></td>
    <td align="center"><sub>147</sub></td>
    <td align="center"><sub>282</sub></td>
    <td align="center"><sub>1.6</sub></td>
    <td align="center"><sub>3.1</sub></td>
    <td align="center"><sub>6.0</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>64</sub></td>
    <td align="center"><sub>43</sub></td>
    <td align="center"><sub>75</sub></td>
    <td align="center"><sub>149</sub></td>
    <td align="center"><sub>273</sub></td>
    <td align="center"><sub>1.7</sub></td>
    <td align="center"><sub>3.5</sub></td>
    <td align="center"><sub>6.4</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>64+64</sub></td>
    <td align="center"><sub>25</sub></td>
    <td align="center"><sub>44</sub></td>
    <td align="center"><sub>84</sub></td>
    <td align="center"><sub>162</sub></td>
    <td align="center"><sub>1.8</sub></td>
    <td align="center"><sub>3.4</sub></td>
    <td align="center"><sub>6.5</sub></td>
  </tr>
  <tr>
    <td align="center" rowspan="3"><sub>SSE</sub></td>
    <td align="center"><sub>32</sub></td>
    <td align="center"><sub>91</sub></td>
    <td align="center"><sub>179</sub></td>
    <td align="center"><sub>352</sub></td>
    <td align="center"><sub>687</sub></td>
    <td align="center"><sub>2.0</sub></td>
    <td align="center"><sub>3.9</sub></td>
    <td align="center"><sub>7.6</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>64</sub></td>
    <td align="center"><sub>50</sub></td>
    <td align="center"><sub>94</sub></td>
    <td align="center"><sub>185</sub></td>
    <td align="center"><sub>361</sub></td>
    <td align="center"><sub>1.9</sub></td>
    <td align="center"><sub>3.7</sub></td>
    <td align="center"><sub>7.2</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>64+64</sub></td>
    <td align="center"><sub>35</sub></td>
    <td align="center"><sub>70</sub></td>
    <td align="center"><sub>139</sub></td>
    <td align="center"><sub>260</sub></td>
    <td align="center"><sub>2.0</sub></td>
    <td align="center"><sub>4.0</sub></td>
    <td align="center"><sub>7.4</sub></td>
  </tr>
  <tr>
    <td align="center" rowspan="3"><b><sub>AVX2</sub></b></td>
    <td align="center"><sub>32</sub></td>
    <td align="center"><sub>126</sub></td>
    <td align="center"><sub>248</sub></td>
    <td align="center"><sub>495</sub></td>
    <td align="center"><sub>950</sub></td>
    <td align="center"><sub>2.0</sub></td>
    <td align="center"><sub>3.9</sub></td>
    <td align="center"><sub>7.5</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>64</sub></td>
    <td align="center"><sub>48</sub></td>
    <td align="center"><sub>95</sub></td>
    <td align="center"><sub>189</sub></td>
    <td align="center"><sub>369</sub></td>
    <td align="center"><sub>2.0</sub></td>
    <td align="center"><sub>3.9</sub></td>
    <td align="center"><sub>7.7</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>64+64</sub></td>
    <td align="center"><sub>34</sub></td>
    <td align="center"><sub>70</sub></td>
    <td align="center"><sub>137</sub></td>
    <td align="center"><sub>254</sub></td>
    <td align="center"><sub>2.1</sub></td>
    <td align="center"><sub>4.0</sub></td>
    <td align="center"><sub>7.5</sub></td>
  </tr>
  <tr>
    <td align="center" rowspan="3"><b><sub>AVX-512</sub></b></td>
    <td align="center"><sub>32</sub></td>
    <td align="center"><sub>149</sub></td>
    <td align="center"><sub>286</sub></td>
    <td align="center"><sub>565</sub></td>
    <td align="center"><sub>1062</sub></td>
    <td align="center"><sub>1.9</sub></td>
    <td align="center"><sub>3.8</sub></td>
    <td align="center"><sub>7.1</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>64</sub></td>
    <td align="center"><sub>65</sub></td>
    <td align="center"><sub>122</sub></td>
    <td align="center"><sub>242</sub></td>
    <td align="center"><sub>462</sub></td>
    <td align="center"><sub>1.9</sub></td>
    <td align="center"><sub>3.7</sub></td>
    <td align="center"><sub>7.1</sub></td>
  </tr>
  <tr>
    <td align="center"><sub>64+64</sub></td>
    <td align="center"><sub>27</sub></td>
    <td align="center"><sub>53</sub></td>
    <td align="center"><sub>105</sub></td>
    <td align="center"><sub>197</sub></td>
    <td align="center"><sub>2.0</sub></td>
    <td align="center"><sub>3.9</sub></td>
    <td align="center"><sub>7.3</sub></td>
  </tr>
</tbody>
</table>
