{
  description = "Flake utils demo";

  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachSystem [ "x86_64-linux" ] (system:
      let pkgs = nixpkgs.legacyPackages.${system}; in
      {
        packages = rec {
          qsma-sequential-native = pkgs.stdenv.mkDerivation rec {
            name = "qsma_sequential";
            src = ./sequential_code;
            installPhase = ''
              runHook preInstall

              mkdir --parent -- $out/
              cp --recursive -- bin $out/

              runHook postInstall
            '';
          };
          qsma-sequential-cross = pkgs.pkgsCross.aarch64-multiplatform-musl.stdenv.mkDerivation rec {
            name = "qsma_sequential";
            src = ./sequential_code;
            installPhase = ''
              runHook preInstall

              mkdir --parent -- $out/
              cp --recursive -- bin $out/

              runHook postInstall
            '';
          };
        };
        devShells.default = pkgs.mkShell {
          nativeBuildInputs = with pkgs; [
            qemu
          ];
        };
      }
    );
}
