(parameterize ([compile-profile #t])
  (load "sim.ss"))

(profile-dump-html)
