module Main where

import Debug.Trace
import Data.List
import qualified Data.Vector.Unboxed as U

worldX = 10
worldY = 10
absorptionRadius = 4

data Matrix = Matrix { rows :: Int
                     , cols :: Int
                     , v    :: U.Vector Double
                     } deriving Show

data Organism = Organism { x :: Int
                         , y :: Int
                         , g :: Double
                         }

zeroMatrix :: Matrix
zeroMatrix = Matrix { rows = worldY, cols = worldX, v = U.replicate (worldX * worldY) 0 }

inBounds :: Int -> Int -> Bool
inBounds x y = x >= 0 && y >= 0 && x < worldX && y < worldY

calculateAttemptedAbsorption :: [Organism] -> Matrix
calculateAttemptedAbsorption = foldl' addOrganismAbsorption zeroMatrix 

addOrganismAbsorption :: Matrix -> Organism -> Matrix
addOrganismAbsorption target o = accum (+) target (nearAbsorption o)

nearAbsorption :: Organism -> [(Int, Double)]
nearAbsorption o = filter (fst . inBounds)  (x o) - absorptionRadius
                   (x o) + absorptionRadius

--addRelativeMatrix :: Matrix -> Int -> Int -> Matrix -> Matrix
--addRelativeMatrix rel x y target = accum (+) target 
----traceShow thing1 $ traceShow thing2 undefined -- foldl' (\a x -> foldl' (\a y -> undefined ) a thing2) target thing1 
--  where filterInMatrix n = filter (\x -> ((n > x) && (0 <= x)))
--        thing2 = filterInMatrix (U.length (B.head target)) $ map (+x) [0..U.length $ B.head target]
--        thing1 = filterInMatrix (B.length target)        $ map (+y) [0..B.length rel]
